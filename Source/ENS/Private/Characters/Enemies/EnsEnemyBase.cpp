// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Characters/Enemies/EnsEnemyBase.h"
#include "Characters/Player/EnsPlayerController.h"
#include "GAS/AttributeSets/EnsHealthAttributeSet.h"
#include "GAS/EnsAbilitySystemComponent.h"
#include "Interactions/EnsMouseInteractableComponent.h"
#include "UI/EnsFloatingInfosBarWidget.h"
#include "UI/EnsFloatingInfosBarWidgetComponent.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

DEFINE_LOG_CATEGORY(LogEnemy)

AEnsEnemyBase::AEnsEnemyBase()
{
    // Actor team
    TeamId = FGenericTeamId(1);
    TeamId.ResetAttitudeSolver();

    // UI
    FloatingInfosBarWidgetComponent = CreateDefaultSubobject<UEnsFloatingInfosBarWidgetComponent>(FName("FloatingStatusBarComponent"));
    FloatingInfosBarWidgetComponent->SetupAttachment(RootComponent);
    FloatingInfosBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 120));
    FloatingInfosBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    FloatingInfosBarWidgetComponent->SetDrawSize(FVector2d(500, 500));

    // Interactions
    InteractZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollider"));
    InteractZone->SetupAttachment(RootComponent);

    InteractionClickZone = CreateDefaultSubobject<UBoxComponent>(TEXT("ClickCollider"));
    InteractionClickZone->SetupAttachment(RootComponent);

    MouseInteractableComponent = CreateDefaultSubobject<UEnsMouseInteractableComponent>(TEXT("Interactions"));
    MouseInteractableComponent->SetupInteractZone(InteractZone, InteractionClickZone);
}

void AEnsEnemyBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
    if (TeamId != NewTeamID)
        TeamId = NewTeamID;
}

FGenericTeamId AEnsEnemyBase::GetGenericTeamId() const
{
    return TeamId;
}

// Called when the game starts or when spawned
void AEnsEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    if (!AbilitySystemComponent)
    {
        UE_LOG(LogEnemy, Error, TEXT("Cannot initialize enemy %s with no AbilitySystemComponent"), *GetName());
        return;
    }

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    AddStartupEffects();

    if (!FloatingInfosBarWidgetClass)
    {
        UE_LOG(LogEnemy, Error, TEXT("Failed to configure UI for enemy %s. Check the UI class is selected in blueprint."), *GetName());
        return;
    }

    FloatingInfosBarWidgetComponent->AddWidget(FloatingInfosBarWidgetClass);
    FloatingInfosBarWidgetComponent->SetHealthPercentage(1.f);

    // Callbacks
    MouseInteractableComponent->OnInteract.AddDynamic(this, &AEnsEnemyBase::Attacked);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute())
        .AddUObject(this, &AEnsEnemyBase::HealthChanged);
}

void AEnsEnemyBase::HealthChanged(const FOnAttributeChangeData& Data)
{
    const float Health = Data.NewValue;

    // Update floating status bar
    if (FloatingInfosBarWidgetComponent)
        FloatingInfosBarWidgetComponent->SetHealthPercentage(Health / HealthAttributeSet->GetMaxHealth());
}

void AEnsEnemyBase::OnDeath()
{
    OnDeath_Blueprint();
    Destroy();
    OnEnemyDestroyed.Broadcast();
}

void AEnsEnemyBase::Attacked(AActor* Source)
{
    const auto* PlayerController = Cast<AEnsPlayerController>(Source);
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("Enemy %s attacked by something that is not a player. Ignoring."), *GetName());
        return;
    }

    Cast<AEnsCharacterBase>(PlayerController->GetCharacter())->BaseAttack();
}
