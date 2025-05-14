// Copyright (c), Firelight Technologies Pty, Ltd.

#pragma once

#include "Commandlets/Commandlet.h"
#include "CoreMinimal.h"
#include "FMODGenerateAssetsCommandlet.generated.h"

/**
 *
 */
UCLASS()
class UFMODGenerateAssetsCommandlet : public UCommandlet
{
    GENERATED_UCLASS_BODY()

    //~ Begin UCommandlet Interface
    virtual int32 Main(const FString& Params) override;
    //~ End UCommandlet Interface
};
