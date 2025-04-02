// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "Utils/EnsBlueprintAssert.h"

#include "Blueprint/BlueprintExceptionInfo.h"

void UEnsBlueprintAssert::BlueprintAssert(UObject* Caller, const bool Condition)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) || USE_LOGGING_IN_SHIPPING
    if (Condition)
        return;

    FBlueprintExceptionInfo ExceptionInfo(EBlueprintExceptionType::Breakpoint);
    FBlueprintCoreDelegates::ThrowScriptException(Caller, *FBlueprintContextTracker::Get().GetCurrentScriptStackWritable().Last(), ExceptionInfo);
#endif
}
