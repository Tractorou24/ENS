// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "Characters/Enemies/EnsEnemyBase.h"
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

/// \brief Delegate for when the player enters the interaction zone.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterZone);

/// \brief Delegate for when the player exits the interaction zone.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitZone);

/**
 * \brief A component that triggers an interaction when clicking on it.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENS_API UEnsMouseInteractableComponent : public UActorComponent, public IEnsInteractable
{
    GENERATED_BODY()

public:
    /// \brief Constructs the component and enables ticking.
    explicit UEnsMouseInteractableComponent();

    /**
     * \brief Checks if the player mouse is over the object.
     * \return `true` if the player's mouse is over the object, `false` otherwise.
     */
    [[nodiscard]] bool IsMouseHovering() const;

    /**
     * \brief Checks if the player is in the interaction zone object.
     * \return `true` if the player is in the interaction zone, `false` otherwise.
     */
    [[nodiscard]] bool IsPlayerInZone() const;

    /**
     * \brief Sets up the interaction zone.
     * \param InteractionZone The box component to use as the interaction zone.
     * \param ClickZone The box component to use as the click zone.
     */
    UFUNCTION(BlueprintCallable)
    void SetupInteractZone(UBoxComponent* InteractionZone, UBoxComponent* ClickZone);

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

    /// \brief Called when the player enters the interaction zone.
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnPlayerEnterZone OnPlayerEnterZone;

    /// \brief Called when the player exits the interaction zone.
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnPlayerExitZone OnPlayerExitZone;

private:
    virtual void Interact_Implementation(AActor* SourceActor) override;

    /// \brief Called when an object is entering the interaction zone.
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                        AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex,
                        bool bFromSweep,
                        const FHitResult& SweepResult);

    /// \brief Called when an object is exiting the interaction zone.
    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    /// \brief Whether the player's mouse is over the object.
    UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
    bool bIsMouseHovering = false;

    /// \brief Whether the player's mouse is over the object.
    UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
    bool bIsPlayerInZone = false;

    /// \brief The box the player must be in to interact with the object
    UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* InteractZone = nullptr;

    /// \brief The box the player must click into to start an interaction
    UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* InteractClickZone = nullptr;
};
