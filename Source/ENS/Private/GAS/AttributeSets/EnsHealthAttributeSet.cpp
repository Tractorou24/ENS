// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "GAS/AttributeSets/EnsHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Characters/EnsCharacterBase.h"

DEFINE_LOG_CATEGORY(LogHealthAttributeSet);

void UEnsHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the damage and reset the meta attribute
		const float DamageAmount = GetDamage();
		SetDamage(0.f);

		if (DamageAmount > 0.f)
		{
			// Apply damage to health
			const float NewHealth = GetHealth() - DamageAmount;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			if (GetHealth() == 0)
			{
				if (AEnsCharacterBase* Character = Cast<AEnsCharacterBase>(GetOwningAbilitySystemComponent()->GetAvatarActor()))
					Character->Death();
				else
					UE_LOG(LogHealthAttributeSet, Error, TEXT("Actor can't be killed."))
			}
			UE_LOG(LogHealthAttributeSet, Display, TEXT("Updated Health : %f"), Health.GetCurrentValue())
		}
	}
}
