// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "Utils/DebugMessageLogRedirector.h"

FDebugMessageLogRedirector::FDebugMessageLogRedirector()
{
    check(GLog);

    DisplayConfigVariable = IConsoleManager::Get().RegisterConsoleVariable(TEXT("ens.RedirectLogAsDebugMsg"),
                                                                           0,
                                                                           TEXT("Defines the minimum log verbosity to redirect to the game debug screen using `GEngine->AddOnScreenDebugMessage`.\n")
                                                                               TEXT("<=0: off(default)\n")
                                                                                   TEXT(" 1: warning\n")
                                                                                       TEXT(" >=2: all\n"));

    GLog->AddOutputDevice(this);
}

FDebugMessageLogRedirector::~FDebugMessageLogRedirector()
{
    if (GLog)
    {
        GLog->RemoveOutputDevice(this);
    }
}

void FDebugMessageLogRedirector::Serialize(const TCHAR* V, const ELogVerbosity::Type Verbosity, const FName& Category)
{
    // Check if the engine is available (in game?) or the redirection is disabled
    if (!GEngine || DisplayConfigVariable->GetInt() <= 0)
        return;

    // Filter log verbosity
    if (DisplayConfigVariable->GetInt() == 1 && Verbosity < ELogVerbosity::Warning)
        return;

    // Output to the game debug screen
    FColor Color = FColor::White;
    if (Verbosity <= ELogVerbosity::Error)
        Color = FColor::Red;
    else if (Verbosity == ELogVerbosity::Warning)
        Color = FColor::Yellow;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, V);
}
