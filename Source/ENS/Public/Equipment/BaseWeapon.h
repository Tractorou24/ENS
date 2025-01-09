// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "BaseWeapon.generated.h"

UCLASS()
class ENS_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseWeapon();

    /// \brief Gameplay ability class reference for the weapon's basic attack
    UPROPERTY(EditAnywhere, BlueprintReadOnly);
    TSubclassOf<class UEnsGameplayAbilityBase> BaseAttack;

    /// \brief Gameplay ability class reference for the weapon's main special ability
    UPROPERTY(EditAnywhere, BlueprintReadOnly);
    TSubclassOf<class UEnsGameplayAbilityBase> MainAbility;

    // \brief Static mesh component representing the weapon's 3D model
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    /// \brief Name of the socket where the weapon should be attached
    UPROPERTY(EditAnywhere)
    FName SocketName;

    /// \brief Data for the different levels of the weapon
    UPROPERTY(EditAnywhere)
    class UDataTable* WeaponInfo;

    /// \brief Level of the weapon
    int Level = 0;
};
