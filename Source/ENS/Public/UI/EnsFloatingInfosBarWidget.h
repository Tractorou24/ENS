// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "EnsFloatingInfosBarWidget.generated.h"

/**
 * \brief The widget responsible to display the player information on screen.
 */
UCLASS()
class ENS_API UEnsFloatingInfosBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /**
     * \brief Set the health bar value.
     * \param HealthPercentage The health ratio in range `[0;1]`, usually `CurrentHealth / MaxHealth`.
     */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthPercentage(float HealthPercentage);
};
