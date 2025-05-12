// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "Characters/Player/EnsPlayerController.h"
#include "Characters/Player/EnsPlayerCharacter.h"
#include "Interactions/EnsInteractable.h"
#include "Interactions/EnsMouseInteractableComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/NavMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AEnsPlayerController::AEnsPlayerController()
{
    // Show mouse cursor, so we can click to move
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;
}

void AEnsPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnsPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    auto* PlayerCharacter = Cast<AEnsPlayerCharacter>(GetCharacter());
    if (!PlayerCharacter)
    {
        UE_LOG(LogPlayerController, Error, TEXT("Failed to get the player character for setting actor abilities."));
        return;
    }

    PlayerCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacter, InPawn);

    PlayerCharacter->OnDeathDelegate.AddUniqueDynamic(this, &AEnsPlayerController::OnDeath);
}

void AEnsPlayerController::OnDeath_Implementation()
{
    if (Lives > 1)
    {
        Lives--;
        LostLife();
    }
    else
        Restart();
}

void AEnsPlayerController::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Update mouse position (if in game)
    if (GetWorld() && (GetWorld()->WorldType == EWorldType::Game || GetWorld()->WorldType == EWorldType::GamePreview || GetWorld()->WorldType == EWorldType::PIE))
    {
        // Get the current viewport size
        FVector2D ViewportSize;
        if (!GEngine && !GEngine->GameViewport)
        {
            UE_LOG(LogPlayerController, Error, TEXT("Failed to get the viewport size. No engine or viewport."));
            return;
        }
        GEngine->GameViewport->GetViewportSize(ViewportSize);

        GetMousePosition(RelativeMousePosition.X, RelativeMousePosition.Y);
        RelativeMousePosition /= ViewportSize;
    }

    // Check interaction
    if (PendingInteractObject)
    {
        // If it is a component, check if the player is in zone before interacting (else wait to be in)
        if (const auto* Component = Cast<UEnsMouseInteractableComponent>(PendingInteractObject))
        {
            if (Component->IsPlayerInZone())
            {
                IEnsInteractable::Execute_Interact(PendingInteractObject, this);
                PendingInteractObject = nullptr;
            }
        }
        else // If it's an interactable directly, interact
        {
            IEnsInteractable::Execute_Interact(PendingInteractObject, this);
            PendingInteractObject = nullptr;
        }
    }
}

void AEnsPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        Subsystem->AddMappingContext(DefaultMappingContext, 0);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    if (!EnhancedInputComponent)
    {
        UE_LOG(LogPlayerController, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
        return;
    }
    EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AEnsPlayerController::Interact);
    EnhancedInputComponent->BindActionValueLambda(InteractAction, ETriggerEvent::Completed, [&](auto _) { bIsInInteractMode = false; });

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEnsPlayerController::Move);
    EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AEnsPlayerController::Attack);
}

void AEnsPlayerController::Move(const FInputActionValue& Value)
{
    auto* PlayerCharacter = Cast<AEnsPlayerCharacter>(GetCharacter());
    PlayerCharacter->Move(Value.Get<FVector2D>());
}

void AEnsPlayerController::Attack()
{
    Cast<AEnsPlayerCharacter>(GetCharacter())->BaseAttack();
}

void AEnsPlayerController::Interact()
{
    FVector CursorWorldLocation;
    FVector CursorWorldDirection;
    if (DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection))
    {
        constexpr float TraceDistance = 1000.0f;
        const FVector TraceEnd = CursorWorldLocation + (CursorWorldDirection * TraceDistance);

        TArray<FHitResult> HitResults;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(GetPawn()); // Ignore the player

        // Perform the sphere trace

        if (const bool bHit = GetWorld()->SweepMultiByChannel(
                HitResults,
                CursorWorldLocation,
                TraceEnd,
                FQuat::Identity,
                ECustomCollisionChannel::ECC_Interactable,
                FCollisionShape::MakeSphere(InteractionRadius),
                QueryParams);
            !bHit)
            return;

        HitResults.Sort([CursorWorldLocation](const FHitResult& hit1, const FHitResult& hit2) {
            return FVector::DistXY(hit1.Location, CursorWorldLocation) < FVector::DistXY(hit2.Location, CursorWorldLocation);
        });

        const FHitResult& ClosestResult = HitResults[0];

        bool IsComponent = false;
        // Is the object interactable directly
        if (ClosestResult.GetActor()->Implements<UEnsInteractable>())
            PendingInteractObject = ClosestResult.GetActor();
        else
        // Is the object interactable by a component
        {
            for (const auto InteractableComponents = ClosestResult.GetActor()->GetComponentsByInterface(UEnsInteractable::StaticClass()); auto* Component : InteractableComponents)
                if (Component->Implements<UEnsInteractable>())
                    PendingInteractObject = Component;
            IsComponent = true;
        }

        // Move to the object
        /* if (const auto* Component = Cast<UEnsMouseInteractableComponent>(PendingInteractObject); IsComponent && !Component->IsPlayerInZone())
            Cast<AEnsPlayerCharacter>(GetCharacter())->MoveToActor(Component->GetOwner());
        else if (!IsComponent)
            Cast<AEnsPlayerCharacter>(GetCharacter())->MoveToActor(ClosestResult.GetActor()); */
        bIsInInteractMode = true;
    }
}

void AEnsPlayerController::ResetInteract(const bool bResetMovement)
{
    bIsInInteractMode = false;
    PendingInteractObject = nullptr;

    if (bResetMovement)
    {
        if (UNavMovementComponent* NavMovementComponent = GetPawn()->GetComponentByClass<UNavMovementComponent>())
        {
            // Cache the velocity
            const FVector CachedVelocity = NavMovementComponent->Velocity;

            // Stop all movements so player can switch between both movement systems
            NavMovementComponent->StopActiveMovement();
            // Put back velocity so movements do not look weird
            NavMovementComponent->Velocity = CachedVelocity;
        }
    }
}
