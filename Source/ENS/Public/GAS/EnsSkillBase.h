// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "GAS/EnsGameplayAbilityBase.h"

#include "CoreMinimal.h"

#include "EnsSkillBase.generated.h"

/**
 * \brief Base class for al weapon/armor skills.
 *
 * It is only used as a common inheritance point for all skills and are them implemented in blueprint.
 */
UCLASS()
class ENS_API UEnsSkillBase : public UEnsGameplayAbilityBase
{
    GENERATED_BODY()

public:
    /// \brief The damage applied at each level of the skill. (0 for the base attack, 1 for the first level, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<float> DamagePerLevel;

    /// \brief The type of actor to spawn (defines collision & range)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill");
    TSubclassOf<AActor> CollisionActor;

    /// \brief The animation montage to play when the skill is activated (at any point defined in the skill, not only at the beginning).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UAnimMontage* AnimationMontage;
};
