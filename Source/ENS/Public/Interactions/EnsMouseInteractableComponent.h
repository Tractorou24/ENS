// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Interactions/EnsInteractable.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnsMouseInteractableComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInteractions, Log, All);

/// \brief Delegate for when the player's mouse is over the object.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseHoverStart);

/// \brief Delegate for when the player is interacting with the object.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseInteract, AActor*, SourceActor);

/// \brief Delegate for when the player's mouse is no longer over the object.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseHoverEnd);

/**
 * \brief A component that triggers an interaction when clicking on it.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENS_API UEnsMouseInteractableComponent : public UActorComponent, public IEnsInteractable
{
	GENERATED_BODY()

public:
	/// \brief Constructs the component and enables ticking.
	explicit UEnsMouseInteractableComponent();

	/**
     * \brief Called every frame. Checks if the player's mouse is over the object.
     * \param DeltaTime The time since the last frame in seconds.
     * \param TickType The type of tick.
     * \param ThisTickFunction The tick function.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// \brief Called when the player's mouse is over the object. (e.g. the player moved the mouse on the object)
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnMouseHoverStart OnMouseHoverStart;

	/// \brief Called when the player is interacting with the object. (e.g. the player clicked on the object)
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnMouseInteract OnInteract;

	/// \brief Called when the player's mouse is no longer over the object. (e.g. the player moved the mouse away)
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnMouseHoverEnd OnMouseHoverEnd;

private:
	virtual void Interact_Implementation(AActor* SourceActor) override;

	/**
	 * \brief Checks if the player mouse is over the object.
     * \return `true` if the player's mouse is over the object, `false` otherwise.
	 */
	[[nodiscard]] bool IsMouseHovering() const;

private:
	/// \brief Whether the player's mouse is over the object.
	UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsMouseHovering = false;
};
