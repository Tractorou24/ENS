// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Interactions/EnsMouseInteractableComponent.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogInteractions)

UEnsMouseInteractableComponent::UEnsMouseInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnsMouseInteractableComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if the object is below the mouse cursor
	const auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (FHitResult Hit; PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit))
	{
		if (Hit.GetActor() == GetOwner())
		{
			if (!bIsMouseHovering)
			{
				bIsMouseHovering = true;
				OnMouseHoverStart.Broadcast();
			}
		} else if (bIsMouseHovering)
		{
			bIsMouseHovering = false;
			OnMouseHoverEnd.Broadcast();
		}
	}
}

void UEnsMouseInteractableComponent::Interact_Implementation(AActor* SourceActor)
{
	IEnsInteractable::Interact_Implementation(SourceActor);
	OnInteract.Broadcast(SourceActor);
}

bool UEnsMouseInteractableComponent::IsMouseHovering() const
{
	return bIsMouseHovering;
}
