// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"

#include "EnsPlayerController.generated.h"

/// \brief The custom collision channels used in the game.
namespace ECustomCollisionChannel
{
    /// \brief The default channel for the player character.
    static constexpr ECollisionChannel ECC_Floor = ECC_GameTraceChannel1;
    /// \brief The channel for the player character's abilities.
    static constexpr ECollisionChannel ECC_Interactable = ECC_GameTraceChannel2;
} // namespace ECustomCollisionChannel

/**
 * \brief The controller used in \ref AEnsPlayerCharacter.
 */
UCLASS()
class ENS_API AEnsPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    explicit AEnsPlayerController();

    /**
     * \brief Called every frame.
     * \param DeltaSeconds The time since the last frame in seconds.
     */
    virtual void Tick(float DeltaSeconds) override;

    /// \brief Returns the current mouse position relative to the viewport or the last known position.
    FVector2D GetRelativeMousePosition() const { return RelativeMousePosition; }

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    /// \brief Binds the inputs to the corresponding player actions.
    virtual void SetupInputComponent() override;

private:
    /// \brief The default mapping context.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* DefaultMappingContext = nullptr;

    /// \brief The current mouse position relative to the viewport.
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
    FVector2D RelativeMousePosition = FVector2D::ZeroVector;

#pragma region Move to destination
    /**
     * \brief Called while \ref SetDestinationAction is triggered/pressed.
     *
     * Sets the new \ref CachedDestination and moves the player in direction it.
     */
    void SetDestinationTriggered();

    /**
     * \brief Called when \ref SetDestinationAction is released.
     * \param InputActionInstance The input action instance.
     *
     * When the click duration is less that \ref ShortPressThreshold, the player will move to the destination, else stop.
     */
    void SetDestinationReleased(const FInputActionInstance& InputActionInstance);

    /// \brief The action to set the player destination when clicking.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* SetDestinationAction = nullptr;

    /// \brief The time threshold (in s) to determine if the click was short (move to when released) or long (stop when released).
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    float ShortPressThreshold = 1;

    /// \brief The destination the player will or has move to.
    FVector CachedDestination = FVector::ZeroVector;
#pragma endregion

#pragma region Interactions
    /// \brief Moves to and interacts with the object under the cursor if it exists.
    void Interact();

    /// \brief Resets the interaction when the action is completed.
    void ResetInteract();

    /// \brief The action to interact with the objects.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* InteractAction = nullptr;

    /// \brief The object the player is trying to interact with.
    UObject* PendingInteractObject = nullptr;

    /// \brief `true` if an interaction was done in the frame so the player movement from click can be disabled.
    bool bIsInInteractMode = false;
#pragma endregion

#pragma region MeshVisibility

public:
    /// \brief Speed at which meshes fade in when no longer occluding the camera.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Components")
    float FadeInSpeed = 5.0f;

    /// \brief Speed at which meshes fade out when occluding the camera.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Components")
    float FadeOutSpeed = 3.0f;

    /// \brief Minimum opacity value for occluding meshes (0.0 = fully transparent, should be > 0 to remain visible).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Components")
    float MinOpacity = 0.1f;

    /// \brief Maximum opacity value for non-occluding meshes (1.0 = fully opaque)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Components")
    float MaxOpacity = 1.f;

private:
    /**
     * \brief Collects all StaticMeshComponents from the children of the specified actor
     * \param Actor The root Actor to search from
     * \return Array of StaticMeshComponents from the children of the specified actor
     */
    TArray<class UStaticMeshComponent*> GetChildrenOfActor(const AActor* Actor);

    /**
     * \brief Handle the fading of objects between the player and camera
     * \param DeltaSeconds The delta time in seconds since the last frame.
     */
    void FadeMesh(float DeltaSeconds);

    /// \brief Maps StaticMeshComponents to their current opacity values
    TMap<class UStaticMeshComponent*, float> OpacityValues;
#pragma endregion
};
