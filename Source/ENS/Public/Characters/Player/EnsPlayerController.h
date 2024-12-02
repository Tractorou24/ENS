// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "InputActionValue.h"

#include "GameFramework/PlayerController.h"
#include "EnsPlayerController.generated.h"

struct FPathFollowingResult;
DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

/// \brief The custom collision channels used in the game.
namespace ECustomCollisionChannel
{
	/// \brief The default channel for the player character.
	static constexpr ECollisionChannel ECC_Floor = ECC_GameTraceChannel1;
	/// \brief The channel for the player character's abilities.
	static constexpr ECollisionChannel ECC_Interactable = ECC_GameTraceChannel2;
}

/**
 * \brief The controller used in \ref AEnsPlayerCharacter.
 */
UCLASS()
class ENS_API AEnsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/// \brief Constructs the controller.
	explicit AEnsPlayerController();

	/// \brief Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/// \brief Called when the controller starts possessing a pawn.
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * \brief Called every frame.
     * \param DeltaSeconds The time since the last frame in seconds.
	 */
	virtual void Tick(float DeltaSeconds) override;

	/// \brief Returns the current mouse position relative to the viewport or the last known position.
	FVector2D GetRelativeMousePosition() const { return RelativeMousePosition; }

protected:
	/// \brief Binds the inputs to the corresponding player actions.
	virtual void SetupInputComponent() override;

private:
	/// \brief The default mapping context.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext = nullptr;

	/// \brief The current mouse position relative to the viewport.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	FVector2D RelativeMousePosition = FVector2D::ZeroVector;

#pragma region Move to destination
	/**
     * \brief Called while \ref SetDestinationAction is triggered/pressed.
     * \param InputActionValue The input action value.
	 *
     * Sets the new \ref CachedDestination and moves the player in direction it.
	 */
	void SetDestinationTriggered(const FInputActionValue& InputActionValue);

	/**
     * \brief Called when \ref SetDestinationAction is released.
     * \param InputActionValue The input action value.
     *
     * When the click duration is less that \ref ShortPressThreshold, the player will move to the destination, else stop.
	 */
	void SetDestinationReleased(const FInputActionValue& InputActionValue);

	/// \brief The action to set the player destination when clicking.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationAction = nullptr;

	/// \brief The time threshold (in s) to determine if the click was short (move to when released) or long (stop when released).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float ShortPressThreshold = 1;

	/// \brief The destination the player will or has move to.
	FVector CachedDestination = FVector::ZeroVector;

	/// \brief The time the player has been following the destination (in s).
	float DestinationFollowTime = 0;
#pragma endregion

#pragma region Interactions
	/// \brief Moves to and interacts with the object under the cursor if it exists.
	void Interact();

	/// \brief The action to interact with the objects.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction = nullptr;

	/// \brief The object the player is trying to interact with.
	UObject* PendingInteractObject = nullptr;
#pragma endregion
};
