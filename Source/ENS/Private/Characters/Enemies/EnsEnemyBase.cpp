// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Characters/Enemies/EnsEnemyBase.h"
#include "Characters/Player/EnsPlayerController.h"
#include "GAS/AttributeSets/EnsHealthAttributeSet.h"
#include "GAS/EnsAbilitySystemComponent.h"
#include "Interactions/EnsMouseInteractableComponent.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(LogEnemy)

AEnsEnemyBase::AEnsEnemyBase()
{
    // Actor team
    TeamId = FGenericTeamId(1);
    TeamId.ResetAttitudeSolver();

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

    // Callbacks
    MouseInteractableComponent->OnInteract.AddDynamic(this, &AEnsEnemyBase::Attacked);
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
