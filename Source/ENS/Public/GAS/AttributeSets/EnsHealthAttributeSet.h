// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "EnsAttributeSetBase.h"

#include "EnsHealthAttributeSet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHealthAttributeSet, Log, All);

/**
 * \brief The attributes storage for any object having health inside the GAS.
 */
UCLASS()
class ENS_API UEnsHealthAttributeSet : public UEnsAttributeSetBase
{
    GENERATED_BODY()

public:
    /**
     * \brief Called after a gameplay effect execution to update its attributes
     * \param Data The data related to the gameplay effect
     */
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    /// \brief The current health
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UEnsHealthAttributeSet, Health)

    /// \brief The maximum Health
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UEnsHealthAttributeSet, MaxHealth)

    /// \brief The health regeneration rate
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData HealthRegenRate;
    ATTRIBUTE_ACCESSORS(UEnsHealthAttributeSet, HealthRegenRate)

    /// \brief A meta attribute that will be used to calculate the damage to apply to \ref Health
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UEnsHealthAttributeSet, Damage)

    /// \brief A meta attribute that will be used to calculate the heal to apply to \ref Health
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Heal;
    ATTRIBUTE_ACCESSORS(UEnsHealthAttributeSet, Heal)
};
