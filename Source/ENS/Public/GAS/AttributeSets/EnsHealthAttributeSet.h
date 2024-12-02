// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EnsHealthAttributeSet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHealthAttributeSet, Log, All);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)			\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * \brief The attributes storage for any object having health inside the GAS.
 */
UCLASS()
class ENS_API UEnsHealthAttributeSet : public UAttributeSet
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
};
