// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "Utils/ScalableFloatUtils.h"

#include "ScalableFloat.h"

float UScalableFloatUtils::GetValueAtLevel(const FScalableFloat& Value, const float Level)
{
    return Value.GetValueAtLevel(Level);
}
