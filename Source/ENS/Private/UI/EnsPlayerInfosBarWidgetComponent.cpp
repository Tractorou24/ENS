// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "UI/EnsPlayerInfosBarWidgetComponent.h"
#include "UI/EnsPlayerInfosBarWidget.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogPlayerUI)

void UEnsPlayerInfosBarWidgetComponent::AddWidget(const TSubclassOf<UEnsPlayerInfosBarWidget>& CreatedWidgetClass)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!PlayerController)
        UE_LOG(LogPlayerUI, Error, TEXT("Cannot configure player UI. Controller is not found in world."));

    PlayerInfosBarWidget = CreateWidget<UEnsPlayerInfosBarWidget>(PlayerController, CreatedWidgetClass);
    if (PlayerInfosBarWidget)
        SetWidget(PlayerInfosBarWidget);
}

void UEnsPlayerInfosBarWidgetComponent::SetHealthPercentage(const float HealthPercentage) const
{
    PlayerInfosBarWidget->SetHealthPercentage(HealthPercentage);
}
