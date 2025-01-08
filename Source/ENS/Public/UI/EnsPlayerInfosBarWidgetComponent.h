// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"

#include "EnsPlayerInfosBarWidgetComponent.generated.h"

class UEnsPlayerInfosBarWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerUI, Log, All);

/**
 * The component added to the player which contains all the player information.
 */
UCLASS()
class ENS_API UEnsPlayerInfosBarWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    /**
     * \brief Configures the player UI with the \p WidgetClass.
     * \param CreatedWidgetClass The class of the widget to configure the player UI with (must be a subclass of \ref UEnsPlayerInfosBarWidget).
     */
    void AddWidget(const TSubclassOf<UEnsPlayerInfosBarWidget>& CreatedWidgetClass);

    /// \copydoc UEnsPlayerInfosBarWidget::SetHealthPercentage()
    void SetHealthPercentage(float HealthPercentage) const;

protected:
    UPROPERTY()
    UEnsPlayerInfosBarWidget* PlayerInfosBarWidget;
};
