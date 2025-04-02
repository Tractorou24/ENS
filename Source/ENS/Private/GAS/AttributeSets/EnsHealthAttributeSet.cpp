// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "GAS/AttributeSets/EnsHealthAttributeSet.h"
#include "Characters/EnsCharacterBase.h"
#include "GameplayEffectExtension.h"

DEFINE_LOG_CATEGORY(LogHealthAttributeSet);

void UEnsHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
    AEnsCharacterBase* Character = Cast<AEnsCharacterBase>(GetOwningAbilitySystemComponent()->GetAvatarActor());
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
            Character->OnHit(DamageAmount);

            if (GetHealth() == 0)
            {
                if (Character)
                    Character->OnDeath();
                else
                    UE_LOG(LogHealthAttributeSet, Error, TEXT("Actor can't be killed."))
            }
            UE_LOG(LogHealthAttributeSet, Display, TEXT("Updated health of actor %s : %f/%f"), *Character->GetName(), Health.GetCurrentValue(), MaxHealth.GetCurrentValue())
        }
    }
    else if (Data.EvaluatedData.Attribute == GetHealAttribute())
    {
        const float HealAmount = GetHeal();
        SetHeal(0.f);

        if (HealAmount > 0.f)
        {
            // Apply damage to health
            const float NewHealth = GetHealth() + HealAmount;
            SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
            UE_LOG(LogHealthAttributeSet, Display, TEXT("Updated health of actor %s : %f/%f"), *Character->GetName(), Health.GetCurrentValue(), MaxHealth.GetCurrentValue())
        }
    }
}
