// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "EnsPlayerInfosBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENS_API UEnsPlayerInfosBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /**
    * @brief Set the UI health bar value
    * @param HealthPercentage Ratio CurrentHealth/MaxHealth
    */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthPercentage(float HealthPercentage);
};
