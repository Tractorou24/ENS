// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "BaseWeapon.generated.h"

/**
 * \brief Base class for any weapon contained in the inventory and be attached to a character.
 */
UCLASS()
class ENS_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();

    /// \brief The weapon's basic attack ability.
    UPROPERTY(EditAnywhere, BlueprintReadOnly);
    TSubclassOf<class UEnsGameplayAbilityBase> BaseAttack;

    /// \brief The weapon's main special ability.
    UPROPERTY(EditAnywhere, BlueprintReadOnly);
    TSubclassOf<class UEnsGameplayAbilityBase> MainAbility;

    /// \brief Component representing the weapon model.
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    /// \brief Name of the socket where the weapon should be attached.
    UPROPERTY(EditAnywhere)
    FName SocketName;
};
