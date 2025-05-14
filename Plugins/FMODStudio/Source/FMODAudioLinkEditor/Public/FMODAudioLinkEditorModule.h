// Copyright (c), Firelight Technologies Pty, Ltd. 2025-2025.

#include "FMODAudioLinkSettings.h"
#include "FMODAudioLinkSettingsFactory.h"
#include "ISettingsModule.h"
#include "Modules/ModuleInterface.h"

class FAssetTypeActions_FMODAudioLinkSettings;

class FFMODAudioLinkEditorModule
{
public:
    FFMODAudioLinkEditorModule()
    {
        // Register asset types
        IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
        {
            AssetActions = MakeShared<FAssetTypeActions_FMODAudioLinkSettings>();
            AssetTools.RegisterAssetTypeActions(AssetActions.ToSharedRef());

            if (ISettingsModule* SettingsModule = FModuleManager::Get().GetModulePtr<ISettingsModule>("Settings"))
            {
                SettingsModule->RegisterSettings("Project", "Plugins", "FMOD AudioLink", NSLOCTEXT("FMODAudioLink", "FMOD AudioLink", "FMOD AudioLink"),
                                                 NSLOCTEXT("FMODAudioLink", "Configure AudioLink for FMOD Settings", "Configure AudioLink for FMOD Settings"), GetMutableDefault<UFMODAudioLinkSettings>());
            }
        }
    }
    ~FFMODAudioLinkEditorModule()
    {
        if (FAssetToolsModule::IsModuleLoaded())
        {
            IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
            AssetTools.UnregisterAssetTypeActions(AssetActions.ToSharedRef());
        }
        AssetActions.Reset();
    }

private:
    TSharedPtr<FAssetTypeActions_FMODAudioLinkSettings> AssetActions;
};
