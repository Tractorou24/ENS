// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnsPlayerInfosBarWidgetComponent.generated.h"

class UEnsPlayerInfosBarWidget;

/**
 * 
 */
UCLASS()
class ENS_API UEnsPlayerInfosBarWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()
    
protected:
    UPROPERTY()
    UEnsPlayerInfosBarWidget* PlayerInfosBarWidget;

public:
    void AddWidget(TSubclassOf<UEnsPlayerInfosBarWidget> widgetClass);

    void SetHealthPercentage(float healthPercentage) const;
};
