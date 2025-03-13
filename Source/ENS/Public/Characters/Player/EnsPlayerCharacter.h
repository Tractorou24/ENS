// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Characters/EnsCharacterBase.h"

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"

#include "EnsPlayerCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

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

public:
    virtual void BeginPlay() override;

    /// \brief Constructs a character and setups its components.
    explicit AEnsPlayerCharacter();

    /// \copydoc AEnsCharacterBase::BaseAttack
    UFUNCTION(BlueprintCallable)
    virtual void BaseAttack() override;

    /// \brief Called when the player attack an enemy
    UFUNCTION(BlueprintImplementableEvent)
    void OnBaseAttack();

    /// \brief Cancel the abilities with passed tags
    UFUNCTION(BlueprintCallable)
    void CancelCurrentAbilities(FGameplayTagContainer WithTags);

    /// \copydoc AEnsCharacterBase::OnDeath
    virtual void OnDeath() override;

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

    /**
     * \brief Sets the team the current player is in.
     * \param NewTeamID The id of the new team.
     */
    UFUNCTION(BlueprintCallable)
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

    /// \brief Gets the id of the team the player is currently in.
    UFUNCTION(BlueprintCallable)
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:
    // Attribute changed callbacks
    virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

private:
    void MoveTo(const struct FAIMoveRequest& MoveReq);

    FGenericTeamId TeamId;

    /// \brief Camera boom positioning the camera above the character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UEnsSpringArmComponent* CameraBoom;

    /// \brief The camera used for the top-down view.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;

    /// \brief The widget class for the player interface.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UEnsPlayerInfosBarWidget> PlayerInfosBarWidgetClass;

    /// \brief The widget component for the player interface.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
    class UEnsPlayerInfosBarWidgetComponent* PlayerInfosBarWidgetComponent;

    /// \brief The component for movement using pathfinding
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UPathFollowingComponent* PathFollowingComponent;

    /// \brief The component for managing inventory
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    class UInventory* Inventory;

    /// \brief The attribute set to hold potions data.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
    class UEnsPotionAttributeSet* PotionAttributeSet;

    /// \brief The radius around the target point the character will stop at.
    UPROPERTY(EditAnywhere, Category = "Movement")
    float AcceptanceRadius = 15.0f;

#pragma region BaseAttck
    /// \brief Whether the player is attacking or not.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Attack", meta = (AllowPrivateAccess = "true"))
    bool bIsAttacking = false;

    /// \brief Whether an attack is buffered or not.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Attack", meta = (AllowPrivateAccess = "true"))
    bool bIsAttackBuffered = false;

    /// \brief Whether an attack can be buffered or not. (e.g. between collision is spawned and destroyed)
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Attack", meta = (AllowPrivateAccess = "true"))
    bool bCanBuffer = false;
#pragma endregion
};
