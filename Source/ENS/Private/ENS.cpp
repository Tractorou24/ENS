// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "ENS.h"
#include "Utils/DebugMessageLogRedirector.h"

#include "Modules/ModuleManager.h"
#include "Utils/DebugMessageLogRedirector.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, ENS, "ENS");

struct FEnsModuleInitializer final
{
    inline static FDebugMessageLogRedirector OutputDevice;

    FEnsModuleInitializer()
    {
        FOutputDeviceRedirector::Get()->AddOutputDevice(&OutputDevice);
    }

    ~FEnsModuleInitializer()
    {
        FOutputDeviceRedirector::Get()->RemoveOutputDevice(&OutputDevice);
    }
} GEnsModuleInitializer;
