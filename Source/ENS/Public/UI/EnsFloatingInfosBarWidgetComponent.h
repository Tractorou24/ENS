// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "EnsFloatingInfosBarWidgetComponent.generated.h"

class UEnsFloatingInfosBarWidget;

/**
 *
 */
UCLASS()
class ENS_API UEnsFloatingInfosBarWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    UEnsFloatingInfosBarWidget* FloatingInfosBarWidget;

public:
    void AddWidget(TSubclassOf<UEnsFloatingInfosBarWidget> widgetClass);

    void SetHealthPercentage(float healthPercentage) const;
};
