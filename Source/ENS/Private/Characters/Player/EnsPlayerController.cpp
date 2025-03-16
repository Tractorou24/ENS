// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Characters/Player/EnsPlayerController.h"
#include "Characters/Player/EnsPlayerCharacter.h"
#include "Interactions/EnsInteractable.h"
#include "Interactions/EnsMouseInteractableComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/NavMovementComponent.h"

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

    EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &AEnsPlayerController::StopMovement);
    EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this, &AEnsPlayerController::SetDestinationTriggered);
    EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &AEnsPlayerController::SetDestinationReleased);

    EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AEnsPlayerController::Interact);
    EnhancedInputComponent->BindActionValueLambda(InteractAction, ETriggerEvent::Completed, [&](auto _) { bIsInInteractMode = false; });
}

void AEnsPlayerController::SetDestinationTriggered()
{
    if (IsInputKeyDown(EKeys::LeftShift))
        return;
    if (bIsInInteractMode)
        return;
    ResetInteract(true);

    // There is something valid below cursor
    if (FHitResult Hit; GetHitResultUnderCursor(ECustomCollisionChannel::ECC_Floor, true, Hit))
        CachedDestination = Hit.Location;

    // Move towards the last cached destination
    if (auto* ControlledPawn = GetPawn())
    {
        const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
        ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
    }
}

void AEnsPlayerController::SetDestinationReleased(const FInputActionInstance& InputActionInstance)
{
    if (IsInputKeyDown(EKeys::LeftShift))
    {
        Cast<AEnsPlayerCharacter>(GetCharacter())->BaseAttack();
        return;
    }

    if (bIsInInteractMode)
        return;
    ResetInteract();

    // If it was a short press
    if (InputActionInstance.GetElapsedTime() <= ShortPressThreshold)
        static_cast<AEnsPlayerCharacter*>(GetCharacter())->MoveToLocation(CachedDestination);
}

void AEnsPlayerController::Interact()
{
    if (FHitResult Hit; GetHitResultUnderCursor(ECustomCollisionChannel::ECC_Interactable, true, Hit))
    {
        bool IsComponent = false;
        // Is the object interactable directly
        if (Hit.GetActor()->Implements<UEnsInteractable>())
            PendingInteractObject = Hit.GetActor();
        else
        // Is the object interactable by a component
        {
            const auto InteractableComponents = Hit.GetActor()->GetComponentsByInterface(UEnsInteractable::StaticClass());
            for (auto* Component : InteractableComponents)
                if (Component->Implements<UEnsInteractable>())
                    PendingInteractObject = Component;
            IsComponent = true;
        }

        // Move to the object
        const auto* Component = Cast<UEnsMouseInteractableComponent>(PendingInteractObject);
        if (IsComponent && !Component->IsPlayerInZone())
            Cast<AEnsPlayerCharacter>(GetCharacter())->MoveToActor(Component->GetOwner());
        else if (!IsComponent)
            Cast<AEnsPlayerCharacter>(GetCharacter())->MoveToActor(Hit.GetActor());
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
