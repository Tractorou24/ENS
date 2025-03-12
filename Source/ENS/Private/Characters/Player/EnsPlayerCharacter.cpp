// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Characters/Player/EnsPlayerCharacter.h"
#include "Characters/Enemies/EnsEnemyBase.h"
#include "Characters/Player/EnsSpringArmComponent.h"
#include "Equipment/Inventory.h"
#include "GAS/AttributeSets/EnsHealthAttributeSet.h"
#include "GAS/EnsAbilitySystemComponent.h"
#include "UI/EnsPlayerInfosBarWidget.h"
#include "UI/EnsPlayerInfosBarWidgetComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "GAS/AttributeSets/EnsPotionAttributeSet.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter)

AEnsPlayerCharacter::AEnsPlayerCharacter()
{
    Tags.Add(FName("Player"));

    // Init capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Init movement
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to movement direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowingComponent"));

    // Init camera
    CameraBoom = CreateDefaultSubobject<UEnsSpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->bUsePawnControlRotation = false; // Do not rotate with character
    CameraBoom->bDoCollisionTest = false;        // Do not pull the camera when colliding with objects

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    CameraComponent->SetupAttachment(CameraBoom, UEnsSpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;

    // Add UI Component
    PlayerInfosBarWidgetComponent = CreateDefaultSubobject<UEnsPlayerInfosBarWidgetComponent>(
        FName("PlayerInfosBarComponent"));
    PlayerInfosBarWidgetComponent->SetupAttachment(RootComponent);
    PlayerInfosBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

    Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
    Inventory->OnSwapWeapon.AddUniqueDynamic(this, &AEnsPlayerCharacter::OnSwapWeapon);
    
    PotionAttributeSet = CreateDefaultSubobject<UEnsPotionAttributeSet>(TEXT("PotionAttributeSet"));

    // Set up actor team
    TeamId = FGenericTeamId(0);
    TeamId.ResetAttitudeSolver();
}

void AEnsPlayerCharacter::BaseAttack(AEnsEnemyBase* Enemy)
{
    FGameplayTagContainer Container;
    Container.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.BaseAttack")));
    GetAbilitySystemComponent()->TryActivateAbilitiesByTag(Container);
}

void AEnsPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogPlayerCharacter, Error, TEXT("Cannot initialize enemy %s with no AbilitySystemComponent"), *GetName());
        return;
    }

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    AddStartupEffects();

    if (!PlayerInfosBarWidgetClass)
    {
        UE_LOG(LogEnemy, Error, TEXT("Failed to configure UI for player %s. Check the UI class is selected in blueprint."), *GetName());
        return;
    }

    PlayerInfosBarWidgetComponent->AddWidget(PlayerInfosBarWidgetClass);
    PlayerInfosBarWidgetComponent->SetHealthPercentage(1.f);

    // Callbacks
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute())
        .AddUObject(this, &AEnsPlayerCharacter::HealthChanged);
}

void AEnsPlayerCharacter::OnDeath()
{
    APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DisableInput(PlayerController);

    // Find and move to the player start
    const AActor* PlayerStart = GetWorld()->GetAuthGameMode()->FindPlayerStart(GetController());
    SetActorLocation(PlayerStart->GetActorLocation());
    SetActorRotation(PlayerStart->GetActorRotation());

    EnableInput(PlayerController);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // Call the parent to reset the attributes
    Super::OnDeath();
}

UPathFollowingComponent* AEnsPlayerCharacter::GetPathFollowingComponent() const
{
    return PathFollowingComponent;
}

void AEnsPlayerCharacter::MoveToLocation(const FVector& Location)
{
    TSubclassOf<UNavigationQueryFilter> DefaultNavigationFilterClass;
    FAIMoveRequest MoveReq(Location);
    MoveReq.SetUsePathfinding(true);
    MoveReq.SetAllowPartialPath(true);
    MoveReq.SetProjectGoalLocation(false);
    MoveReq.SetNavigationFilter(DefaultNavigationFilterClass);
    MoveReq.SetAcceptanceRadius(AcceptanceRadius);
    MoveReq.SetReachTestIncludesAgentRadius(false);
    MoveReq.SetCanStrafe(true);
    MoveTo(MoveReq);
}

void AEnsPlayerCharacter::MoveToActor(const AActor* Actor)
{
    TSubclassOf<UNavigationQueryFilter> DefaultNavigationFilterClass;
    FAIMoveRequest MoveReq(Actor);
    MoveReq.SetUsePathfinding(true);
    MoveReq.SetAllowPartialPath(true);
    MoveReq.SetNavigationFilter(DefaultNavigationFilterClass);
    MoveReq.SetAcceptanceRadius(AcceptanceRadius);
    MoveReq.SetReachTestIncludesAgentRadius(true);
    MoveReq.SetCanStrafe(true);
    MoveTo(MoveReq);
}

class UCameraComponent* AEnsPlayerCharacter::GetCameraComponent() const
{
    return CameraComponent;
}

void AEnsPlayerCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
    if (TeamId != NewTeamID)
    {
        TeamId = NewTeamID;
    }
}

FGenericTeamId AEnsPlayerCharacter::GetGenericTeamId() const
{
    return TeamId;
}

void AEnsPlayerCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
    const float Health = Data.NewValue;
    const float MaxHealth = HealthAttributeSet->GetMaxHealth();

    // Update floating status bar
    if (PlayerInfosBarWidgetComponent)
        PlayerInfosBarWidgetComponent->SetHealthPercentage(Health / MaxHealth);
}

void AEnsPlayerCharacter::MoveTo(const FAIMoveRequest& MoveReq)
{
    FPathFindingQuery PFQuery;
    if (!MoveReq.IsMoveToActorRequest())
    {
        bool bCanRequestMove = true;

        if (MoveReq.GetGoalLocation().ContainsNaN() || FAISystem::IsValidLocation(MoveReq.GetGoalLocation()) == false)
            bCanRequestMove = false;

        if (bCanRequestMove && MoveReq.IsProjectingGoal())
        {
            UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
            const FNavAgentProperties& AgentProps = GetNavAgentPropertiesRef();
            FNavLocation ProjectedLocation;

            if (NavSys && !NavSys->ProjectPointToNavigation(MoveReq.GetGoalLocation(), ProjectedLocation, INVALID_NAVEXTENT, &AgentProps))
                bCanRequestMove = false;

            MoveReq.UpdateGoalLocation(ProjectedLocation.Location);
        }
    }

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    const ANavigationData* NavData = (NavSys == nullptr)
                                         ? nullptr
                                     : MoveReq.IsUsingPathfinding()
                                         ? NavSys->GetNavDataForProps(GetNavAgentPropertiesRef(), GetNavAgentLocation())
                                         : NavSys->GetAbstractNavData();

    if (NavData)
    {
        FVector GoalLocation = MoveReq.GetGoalLocation();
        if (MoveReq.IsMoveToActorRequest())
        {
            const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveReq.GetGoalActor());
            if (NavGoal)
            {
                const FVector Offset = NavGoal->GetMoveGoalOffset(static_cast<const AActor*>(this));
                GoalLocation = FQuatRotationTranslationMatrix(MoveReq.GetGoalActor()->GetActorQuat(), NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
            }
            else
            {
                GoalLocation = MoveReq.GetGoalActor()->GetActorLocation();
            }
        }
        FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, MoveReq.GetNavigationFilter());
        PFQuery = FPathFindingQuery(*this, *NavData, GetNavAgentLocation(), GoalLocation, NavFilter);
        PFQuery.SetAllowPartialPaths(MoveReq.IsUsingPartialPaths());
        PFQuery.SetRequireNavigableEndLocation(MoveReq.IsNavigableEndLocationRequired());
        if (MoveReq.IsApplyingCostLimitFromHeuristic())
        {
            const float HeuristicScale = NavFilter->GetHeuristicScale();
            PFQuery.CostLimit = FPathFindingQuery::ComputeCostLimitFromHeuristic(PFQuery.StartLocation,
                                                                                 PFQuery.EndLocation,
                                                                                 HeuristicScale,
                                                                                 MoveReq.GetCostLimitFactor(),
                                                                                 MoveReq.GetMinimumCostLimit());
        }

        if (PathFollowingComponent)
        {
            PathFollowingComponent->OnPathfindingQuery(PFQuery);
        }
    }

    FNavPathSharedPtr Path;
    if (NavSys)
    {
        FPathFindingResult PathResult = NavSys->FindPathSync(PFQuery);
        if (PathResult.Result != ENavigationQueryResult::Error)
        {
            if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
            {
                if (MoveReq.IsMoveToActorRequest())
                {
                    PathResult.Path->SetGoalActorObservation(*MoveReq.GetGoalActor(), 100.0f);
                }

                PathResult.Path->EnableRecalculationOnInvalidation(true);
                Path = PathResult.Path;
            }
        }
    }

    Path.IsValid() ? PathFollowingComponent->RequestMove(MoveReq, Path) : FAIRequestID::InvalidRequest;
}
