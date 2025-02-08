// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "GAS/AttributeSets/EnsPotionAttributeSet.h"
#include "Characters/EnsCharacterBase.h"
#include "GameplayEffectExtension.h"

DEFINE_LOG_CATEGORY(LogPotionAttributeSet);

void UEnsPotionAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetPotionUseAttribute())
    {
        const float NewHealthPotionQuantity = GetHealthQuantity() - GetPotionUse();
        //Reset Meta attribute
        SetPotionUse(0.f);
        SetHealthQuantity(FMath::Clamp(NewHealthPotionQuantity, 0.0f, GetHealthMaxQuantity()));
    }
}
