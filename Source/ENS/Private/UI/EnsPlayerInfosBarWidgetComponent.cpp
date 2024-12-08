// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.


#include "UI/EnsPlayerInfosBarWidgetComponent.h"
#include "UI/EnsPlayerInfosBarWidget.h"
#include "Kismet/GameplayStatics.h"

void UEnsPlayerInfosBarWidgetComponent::AddWidget(TSubclassOf<UEnsPlayerInfosBarWidget> widgetClass)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PC)
    {
        PlayerInfosBarWidget = CreateWidget<UEnsPlayerInfosBarWidget>(PC, widgetClass);
        if (PlayerInfosBarWidget)
        {
            SetWidget(PlayerInfosBarWidget);
            PlayerInfosBarWidget->AddToViewport();
        }
    }
}

void UEnsPlayerInfosBarWidgetComponent::SetHealthPercentage(float healthPercentage) const
{
    PlayerInfosBarWidget->SetHealthPercentage(healthPercentage);
}
