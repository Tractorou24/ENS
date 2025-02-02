// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Characters/EnsCharacterBase.h"

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "EnsEnemyBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEnemy, Log, All);

/**
 * \brief Base class for any enemy in the game.
 *
 * This adds:
 *   - the generic UI (health, ...)
 *   - the team agent for the AI system
 *   - the interaction system (which allows to be damaged by the player)
 */
UCLASS()
class ENS_API AEnsEnemyBase : public AEnsCharacterBase, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    explicit AEnsEnemyBase();

    /**
     * \brief Sets the team the current enemy is in.
     * \param NewTeamID The id of the new team.
     */
    UFUNCTION(BlueprintCallable)
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

    /// \brief Gets the id of the team the enemy is currently in.
    UFUNCTION(BlueprintCallable)
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:
    virtual void BeginPlay() override;

    /**
     * \brief Called when the health attribute changes.
     * \param Data The data of the attribute change.
     */
    virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

    /// \brief Kills the enemy.
    virtual void OnDeath() override;

    /**
     * @brief OnDeath blueprint event called by OnDeath native class
     * Unreal does not accepts Blueprint Implementable/Native events to be virtual
     */
    UFUNCTION(BlueprintImplementableEvent)
    void OnDeath_Blueprint();

private:
    /**
     * \brief Called when the enemy is attacked.
     * \param Source The actor that attacked.
     */
    UFUNCTION()
    void Attacked(AActor* Source);

    FGenericTeamId TeamId;

    /// \brief The widget class of the enemy.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UEnsFloatingInfosBarWidget> FloatingInfosBarWidgetClass;

    /// \brief The widget component of the enemy.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
    class UEnsFloatingInfosBarWidgetComponent* FloatingInfosBarWidgetComponent;

    /// \brief The interaction component.
    UPROPERTY(EditAnywhere, Category = "Interactions", meta = (AllowPrivateAccess = "true"))
    class UEnsMouseInteractableComponent* MouseInteractableComponent = nullptr;

    /// \brief The interaction zone of the enemy (i.e. where it can interact after moving)
    UPROPERTY(EditAnywhere, Category = "Interactions", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* InteractZone = nullptr;

    /// \brief The zone the player can click into to interact with the enemy.
    UPROPERTY(EditAnywhere, Category = "Interactions", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* InteractionClickZone = nullptr;
};
