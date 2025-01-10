// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Characters/EnsCharacterBase.h"
#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "EnsPlayerCharacter.generated.h"

/**
 * \brief The player character in the game.
 *
 * It provides basic functionalities and integrates input, movement, camera, abilities...
 * It is the default character in the \ref EnsGameMode.
 */
UCLASS()
class ENS_API AEnsPlayerCharacter : public AEnsCharacterBase, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

    FGenericTeamId TeamId;

public:
    /// \brief Called at the game start
    virtual void BeginPlay() override;

    /// \brief Constructs a character and setups its components.
    explicit AEnsPlayerCharacter();

    /// \copydoc AEnsCharacterBase::BaseAttack
    virtual void BaseAttack(AEnsEnemyBase* Enemy) override;

    /// \copydoc AEnsCharacterBase::Death
    virtual void Death() override;

    /// \brief Gets the path following component of the character used to move on a navmesh.
    [[nodiscard]] class UPathFollowingComponent* GetPathFollowingComponent() const;

    /**
     * \brief Moves the player character to a \p Location.
     * \param Location The location to move to.
     */
    void MoveToLocation(const FVector& Location);

    /**
     * \brief Moves the player character to an \p Actor.
     * \param Actor The actor to move to.
     */
    void MoveToActor(const AActor* Actor);

    /// \brief Gets the camera component the player sees through.
    [[nodiscard]] class UCameraComponent* GetCameraComponent() const;

    UFUNCTION(BlueprintCallable)
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

    UFUNCTION(BlueprintCallable)
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ens|UI")
    TSubclassOf<class UEnsPlayerInfosBarWidget> PlayerInfosBarWidgetClass;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ens|UI")
    class UEnsPlayerInfosBarWidgetComponent* PlayerInfosBarWidgetComponent;

    // Attribute changed callbacks
    virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

    /**
     * \brief Attacks the given \p Target using the currently selected weapon.
     * \param Target The actor to attack.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void BaseAttack(AEnsCharacterBase* Target);

private:
    void MoveTo(const struct FAIMoveRequest& MoveReq);

    /// \brief Camera boom positioning the camera above the character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UEnsSpringArmComponent* CameraBoom;

    /// \brief The camera used for the top-down view.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;

    /// \brief The component for movement using pathfinding
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UPathFollowingComponent* PathFollowingComponent;

    /// \brief The radius around the target point the character will stop at.
    UPROPERTY(EditAnywhere, Category = "Movement")
    float AcceptanceRadius = 15.0f;
};
