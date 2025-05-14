// Copyright Epic Games, Inc. All Rights Reserved.

#include "FMODStudioNiagara.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FFMODStudioNiagaraModule"

void FFMODStudioNiagaraModule::StartupModule()
{
    IPluginManager::Get().FindPlugin(TEXT("FMODStudioNiagara"))->GetBaseDir();
}

void FFMODStudioNiagaraModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFMODStudioNiagaraModule, FMODStudioNiagara)
