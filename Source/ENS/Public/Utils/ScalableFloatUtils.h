// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ScalableFloatUtils.generated.h"

struct FScalableFloat;

/**
 * \brief Helper functions to export C++ only members of \ref FScalableFloat in blueprint.
 */
UCLASS()
class ENS_API UScalableFloatUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * \brief Gets the value of the scalable float \p Value using its curve at \p Level.
     * \param Value The scalable float to get the value from.
     * \param Level The level to get the associated value.
     * \return The value at \ref Level from \p Value.
     */
    UFUNCTION(BlueprintCallable, Category = "Utils")
    static float GetValueAtLevel(const FScalableFloat& Value, float Level);
};
