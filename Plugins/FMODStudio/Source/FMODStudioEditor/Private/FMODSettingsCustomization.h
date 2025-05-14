// Copyright (c), Firelight Technologies Pty, Ltd. 2025.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"
#include "Misc/Attribute.h"
#include "PropertyHandle.h"

class IDetailLayoutBuilder;

class FFMODSettingsCustomization : public IDetailCustomization
{
public:
    // Makes a new instance of this detail layout class for a specific detail view requesting it
    static TSharedRef<IDetailCustomization> MakeInstance();

    // IDetailCustomization interface
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
    // End of IDetailCustomization interface

private:
    FFMODSettingsCustomization();
};
