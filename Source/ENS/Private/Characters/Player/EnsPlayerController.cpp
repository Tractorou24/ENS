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
	ActiveCamera = Cast<UCameraComponent>(GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
	ActiveCapsuleComponent = Cast<UCapsuleComponent>(GetPawn()->GetComponentByClass(UCapsuleComponent::StaticClass()));
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
	
	FVector Start = ActiveCamera->GetComponentLocation();
    FVector End = GetPawn()->GetActorLocation();

    TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
    CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

    TArray<AActor*> ActorsToIgnore;
    TArray<FHitResult> OutHits;

    

    bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(), Start, End, ActiveCapsuleComponent->GetScaledCapsuleRadius(),
        ActiveCapsuleComponent->GetScaledCapsuleHalfHeight(), CollisionObjectTypes, true,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHits, true);

	if(bGotHits)
    {
        TSet<UStaticMeshComponent*> MeshesJustOccluded;
        
        for (FHitResult Hit : OutHits)
        {
            const AActor* HitActor = Cast<AActor>(Hit.GetActor());
            if (!HitActor)
            	continue;

            TArray<UStaticMeshComponent*> GroupMeshes;
        	GetChildsOfActor(HitActor, GroupMeshes);
            
            for (UStaticMeshComponent* MeshComp : GroupMeshes)
            {
                float CurrentOpacity = OpacityValues.Contains(MeshComp) ? OpacityValues[MeshComp] : MaxOpacity;
                float NewOpacity = FMath::FInterpTo(CurrentOpacity, MinOpacity, DeltaSeconds, FadeInSpeed);
                OpacityValues.Add(MeshComp, NewOpacity);
                MeshComp->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), NewOpacity);
                MeshesJustOccluded.Add(MeshComp);
            }
        }
        for (auto It = OpacityValues.CreateIterator(); It; ++It)
        {
            if (!MeshesJustOccluded.Contains(It.Key()))
            {
                float CurrentOpacity = It.Value();
                float NewOpacity = FMath::FInterpTo(CurrentOpacity, MaxOpacity, DeltaSeconds, FadeOutSpeed);
            	
                if (FMath::IsNearlyEqual(NewOpacity, MaxOpacity, 0.01f))
                {
                    It.RemoveCurrent();
                }
                else
                {
                    It.Value() = NewOpacity;
                    It.Key()->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), NewOpacity);
                }
            }
        }
    }
    else
    {
        for (auto It = OpacityValues.CreateIterator(); It; ++It)
        {
            float CurrentOpacity = It.Value();
            float NewOpacity = FMath::FInterpTo(CurrentOpacity, MaxOpacity, DeltaSeconds, FadeOutSpeed);
            
            if (FMath::IsNearlyEqual(NewOpacity, MaxOpacity, 0.01f))
            {
                It.RemoveCurrent();
            }
            else
            {
                It.Value() = NewOpacity;
                It.Key()->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), NewOpacity);
            }
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

bool AEnsPlayerController::GetChildsOfActor(const AActor* Actor, TArray<UStaticMeshComponent*>& Result)
{
	if (!Actor)
		return false;
	
	AActor* Parent = Actor->GetAttachParentActor();
	if (Parent)
	{
		// Add parent's mesh if it has one
		if (UStaticMeshComponent* ParentMesh = Parent->FindComponentByClass<UStaticMeshComponent>())
		{
			Result.Add(ParentMesh);
		}

		// Get all attached actors' meshes
		TArray<AActor*> GroupedActors;
		Parent->GetAttachedActors(GroupedActors);
		for (AActor* tmp: GroupedActors)
		{
			if (UStaticMeshComponent* MeshComp = tmp->FindComponentByClass<UStaticMeshComponent>())
			{
				Result.Add(MeshComp);
			}
		}
	}
	else
	{
		// If not grouped, just add this actor's mesh
		if (UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>())
		{
			Result.Add(MeshComp);
		}
	}
	return true;
}
