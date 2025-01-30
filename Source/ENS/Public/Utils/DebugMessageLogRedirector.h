// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * \brief Redirects log messages to the game debug screen using `GEngine->AddOnScreenDebugMessage`.
 */
class ENS_API FDebugMessageLogRedirector final : public FOutputDevice
{
public:
    explicit FDebugMessageLogRedirector();
    virtual ~FDebugMessageLogRedirector() override;

protected:
    virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override;

private:
    IConsoleVariable* DisplayConfigVariable = nullptr;
};
