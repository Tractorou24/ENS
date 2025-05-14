// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "GAS/AttributeSets/EnsMovementAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogMovementAttributeSet)

FMaxSpeedAttributeData::FMaxSpeedAttributeData(UEnsMovementAttributeSet* AttributeSet)
    : OwningAttributeSet(AttributeSet)
{}

void FMaxSpeedAttributeData::SetCurrentValue(const float NewValue)
{
    FGameplayAttributeData::SetCurrentValue(NewValue);
    OwningAttributeSet->GetOwningActor()->GetComponentByClass<UCharacterMovementComponent>()->MaxWalkSpeed = NewValue;
}

UEnsMovementAttributeSet::UEnsMovementAttributeSet()
    : MaxSpeed(this)
{}
