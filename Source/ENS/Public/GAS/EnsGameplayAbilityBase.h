// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EnsGameplayAbilityBase.generated.h"

/**
 * \brief Base class for all custom abilities in the project.
 */
UCLASS()
class ENS_API UEnsGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/**
     * \brief Gets the controller owing the GAS main component.
     * \return The controller if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	[[nodiscard]] AController* GetControllerFromActorInfo() const;

	/// \brief Sets the cooldown duration to use in ApplyCooldown()
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FScalableFloat CooldownDuration;

	/// \brief Defines the tags that can put this ability in cooldown
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FGameplayTagContainer CooldownTags;

	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

	/**
	 * @brief Get all the possible tags (parent class and this) that can put the ability in cooldown
	 * @return A tag container 
	 */
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	/**
	 * @brief Method automatically called to apply the CooldownEffect if provided
	 * @param Handle The handle of the ability that will be used
	 * @param ActorInfo The actor info stored in the ASC
	 * @param ActivationInfo The info related to the ability activation
	 */
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
};
