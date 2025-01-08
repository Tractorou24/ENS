// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "EnsPlayerInfosBarWidget.generated.h"

/**
 * \brief The UI widget for the player.
 */
UCLASS()
class ENS_API UEnsPlayerInfosBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /**
     * \brief Set the UI health bar value.
     * \param HealthPercentage Ratio `CurrentHealth / MaxHealth`
     */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthPercentage(float HealthPercentage);
};
