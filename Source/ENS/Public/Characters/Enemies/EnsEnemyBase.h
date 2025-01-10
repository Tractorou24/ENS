// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Characters/EnsCharacterBase.h"
#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "EnsEnemyBase.generated.h"

UCLASS()
class ENS_API AEnsEnemyBase : public AEnsCharacterBase, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

    FGenericTeamId TeamId;

public:
    // Sets default values for this character's properties
    AEnsEnemyBase();

    UFUNCTION(BlueprintCallable)
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

    UFUNCTION(BlueprintCallable)
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /**
     * \brief Called when the health attribute changes.
     * \param Data The data of the attribute change.
     */
    virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

    /// \brief Implement death of all enemies.
    virtual void Death() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ens|UI")
    TSubclassOf<class UEnsFloatingInfosBarWidget> FloatingInfosBarWidgetClass;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ens|UI")
    class UEnsFloatingInfosBarWidgetComponent* FloatingInfosBarWidgetComponent;

private:
    /**
     * \brief Called when the enemy is attacked.
     * \param Source The actor that attacked.
     */
    UFUNCTION()
    void Attacked(AActor* Source);

    UPROPERTY(EditAnywhere, Category = "Interactions", meta = (AllowPrivateAccess = "true"))
    class UEnsMouseInteractableComponent* MouseInteractableComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Interactions", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* InteractZone = nullptr;
};
