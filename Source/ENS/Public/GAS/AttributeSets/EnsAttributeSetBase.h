// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "EnsAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * \brief The default attributes storage for any object using GAS.
 */
UCLASS()
class ENS_API UEnsAttributeSetBase : public UAttributeSet
{
    GENERATED_BODY()
public:
    UEnsAttributeSetBase();
};
