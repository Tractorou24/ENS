// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "UI/EnsFloatingInfosBarWidgetComponent.h"
#include "UI/EnsFloatingInfosBarWidget.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogEnemyUI)

void UEnsFloatingInfosBarWidgetComponent::AddWidget(const TSubclassOf<UEnsFloatingInfosBarWidget>& CreatedWidgetClass)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!PlayerController)
        UE_LOG(LogEnemyUI, Error, TEXT("Cannot configure enemy UI. Controller is not found in world."));

    FloatingInfosBarWidget = CreateWidget<UEnsFloatingInfosBarWidget>(PlayerController, CreatedWidgetClass);
    if (FloatingInfosBarWidget)
        SetWidget(FloatingInfosBarWidget);
}

void UEnsFloatingInfosBarWidgetComponent::SetHealthPercentage(const float HealthPercentage) const
{
    FloatingInfosBarWidget->SetHealthPercentage(HealthPercentage);
}
