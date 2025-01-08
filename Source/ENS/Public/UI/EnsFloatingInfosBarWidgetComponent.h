// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "EnsFloatingInfosBarWidgetComponent.generated.h"

class UEnsFloatingInfosBarWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogEnemyUI, Log, All);

/**
 * Component that holds an enemy UI, so it can be easily added to any of them.
 */
UCLASS()
class ENS_API UEnsFloatingInfosBarWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    /**
     * \brief Configures the enemy UI with the \p WidgetClass.
     * \param CreatedWidgetClass The class of the widget to configure the enemy UI with (must be a subclass of \ref UEnsFloatingInfosBarWidget).
     */
    void AddWidget(const TSubclassOf<UEnsFloatingInfosBarWidget>& CreatedWidgetClass);

    /// \copydoc UEnsFloatingInfosBarWidget::SetHealthPercentage()
    void SetHealthPercentage(float HealthPercentage) const;

private:
    UPROPERTY()
    UEnsFloatingInfosBarWidget* FloatingInfosBarWidget;
};
