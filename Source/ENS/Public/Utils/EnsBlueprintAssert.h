// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EnsBlueprintAssert.generated.h"

/**
 * \brief Function library to handle checks and asserts that stops the game in blueprint.
 */
UCLASS()
class ENS_API UEnsBlueprintAssert : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * \brief Stops the execution if \p Condition is `false`.
     * \param Caller The caller object.
     * \param Condition `true` to insert a breakpoint and stop execution, `false` does nothing.
     *
     * \note This is disabled in shipping or test unless explicitly enabled with `USE_LOGGING_IN_SHIPPING`.
     */
    UFUNCTION(BlueprintCallable, Category = "Debbuging")
    static void BlueprintAssert(UObject* Caller, bool Condition);
};
