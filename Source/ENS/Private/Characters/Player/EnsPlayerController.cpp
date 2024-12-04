// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Characters/Player/EnsPlayerController.h"
#include "Characters/Player/EnsPlayerCharacter.h"
#include "Interactions/EnsInteractable.h"
#include "Interactions/EnsMouseInteractableComponent.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Navigation/PathFollowingComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);

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
		UE_LOG(LogPlayerCharacter, Error, TEXT("Failed to get the player character for setting actor abilities."));
		return;
	}

	PlayerCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacter, InPawn);
}

void AEnsPlayerController::FadeMesh(const float DeltaSeconds)
{
	const UCameraComponent* ActiveCameraComponent = Cast<AEnsPlayerCharacter>(GetCharacter())->GetCameraComponent();
	const UCapsuleComponent* ActiveCapsuleComponent = Cast<AEnsPlayerCharacter>(GetCharacter())->GetCapsuleComponent();
	
	TArray<FHitResult> OutHits;
	const bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		GetWorld(),
		ActiveCameraComponent->GetComponentLocation(),
		GetPawn()->GetActorLocation(),
		ActiveCapsuleComponent->GetScaledCapsuleRadius(),
		ActiveCapsuleComponent->GetScaledCapsuleHalfHeight(),
		{UEngineTypes::ConvertToObjectType(ECC_WorldStatic)},
		true,
		{},
		EDrawDebugTrace::None,
		OutHits,
		true);

	TSet<UStaticMeshComponent*> MeshesJustOccluded;
	if (bGotHits)
	{
		for (FHitResult Hit : OutHits)
		{
			// Get all static mesh components from the hit actor's parent
			TArray<UStaticMeshComponent*> GroupMeshes = GetChildrenOfActor(Hit.GetActor()->GetAttachParentActor());

			for (UStaticMeshComponent* MeshComp : GroupMeshes)
			{
				// Skip if we've already processed this mesh this frame
				if (MeshesJustOccluded.Contains(MeshComp))
					continue;

				// Fade the opacity to the minimum value
				float CurrentOpacity = OpacityValues.Contains(MeshComp) ? OpacityValues[MeshComp] : MaxOpacity;
				float NewOpacity = FMath::FInterpTo(CurrentOpacity, MinOpacity, DeltaSeconds, FadeInSpeed);
				OpacityValues.Add(MeshComp, NewOpacity);
				MeshComp->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), NewOpacity);
				MeshesJustOccluded.Add(MeshComp);
			}
		}
	}

	// Process all meshes that were previously fading but not hit this frame
	for (auto& [Mesh, OpacityValue] : OpacityValues)
	{
		if (!MeshesJustOccluded.Contains(Mesh))
		{
			// Fade the opacity back to maximum
			float NewOpacity = FMath::FInterpTo(OpacityValue, MaxOpacity, DeltaSeconds, FadeOutSpeed);
			OpacityValue = NewOpacity;
			Mesh->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), NewOpacity);

			// Remove the entry if opacity has returned to maximum
			if (FMath::IsNearlyEqual(NewOpacity, MaxOpacity, 0.01f))
				OpacityValues.Remove(Mesh);
		}
	}
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
			UE_LOG(LogPlayerCharacter, Error, TEXT("Failed to get the viewport size. No engine or viewport."));
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
		} else // If it's an interactable directly, interact
		{
			IEnsInteractable::Execute_Interact(PendingInteractObject, this);
			PendingInteractObject = nullptr;
		}
	}

	// Handle meshes fading
	FadeMesh(DeltaSeconds);
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
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
		return;
	}

	EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &AEnsPlayerController::StopMovement);
	EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this, &AEnsPlayerController::SetDestinationTriggered);
	EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &AEnsPlayerController::SetDestinationReleased);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AEnsPlayerController::Interact);
}

void AEnsPlayerController::SetDestinationTriggered(const FInputActionValue& InputActionValue)
{
	if (PendingInteractObject)
		return;

	DestinationFollowTime += GetWorld()->GetDeltaSeconds();

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

void AEnsPlayerController::SetDestinationReleased(const FInputActionValue& InputActionValue)
{
	if (PendingInteractObject)
		return;

	// If it was a short press
	if (DestinationFollowTime <= ShortPressThreshold)
		static_cast<AEnsPlayerCharacter*>(GetCharacter())->MoveToLocation(CachedDestination);
	DestinationFollowTime = 0.0f;
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
		if (IsComponent)
			Cast<AEnsPlayerCharacter>(GetCharacter())->MoveToActor(Cast<const UActorComponent>(PendingInteractObject)->GetOwner());
		else
			Cast<AEnsPlayerCharacter>(GetCharacter())->MoveToActor(Hit.GetActor());
	}
}

TArray<UStaticMeshComponent*> AEnsPlayerController::GetChildrenOfActor(const AActor* Actor)
{
	TArray<UStaticMeshComponent*> Childs;

	if (!Actor)
		return Childs;

	TArray<AActor*> GroupedActors;
	Actor->GetAttachedActors(GroupedActors);

	// Get all meshes of attached actors
	for (AActor* tmp : GroupedActors)
		if (UStaticMeshComponent* MeshComp = tmp->FindComponentByClass<UStaticMeshComponent>())
			Childs.Add(MeshComp);
	return Childs;
}
