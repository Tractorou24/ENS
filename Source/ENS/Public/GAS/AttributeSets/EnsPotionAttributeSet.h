// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"

#include "EnsPotionAttributeSet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPotionAttributeSet, Log, All);

/**
 * \brief Defines an attribute in an attribute set. It helps create the getter and setter for the attribute.
 * \param ClassName The class the attribute is set in.
 * \param PropertyName The name of the attribute.
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * \brief The attributes storage for any object having Potion inside the GAS.
 */
UCLASS()
class ENS_API UEnsPotionAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    /**
     * \brief Called after a gameplay effect execution to update its attributes
     * \param Data The data related to the gameplay effect
     */
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    /// \brief The current Health Potion quantity
    UPROPERTY(BlueprintReadOnly, Category = "HealthPotion")
    FGameplayAttributeData HealthQuantity;
    ATTRIBUTE_ACCESSORS(UEnsPotionAttributeSet, HealthQuantity)

    /// \brief The maximum Potion quantity
    UPROPERTY(BlueprintReadOnly, Category = "HealthPotion")
    FGameplayAttributeData HealthMaxQuantity;
    ATTRIBUTE_ACCESSORS(UEnsPotionAttributeSet, HealthMaxQuantity)

    /// \brief A meta attribute that will be used to calculate the number of potion to use (usually 1)
    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData PotionUse;
    ATTRIBUTE_ACCESSORS(UEnsPotionAttributeSet, PotionUse)
};
