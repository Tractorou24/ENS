// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.


#include "UI/EnsFloatingInfosBarWidgetComponent.h"
#include "UI/EnsFloatingInfosBarWidget.h"
#include "Kismet/GameplayStatics.h"

void UEnsFloatingInfosBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnsFloatingInfosBarWidgetComponent::AddWidget(TSubclassOf<UEnsFloatingInfosBarWidget> widgetClass)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC)
	{
		FloatingInfosBarWidget = CreateWidget<UEnsFloatingInfosBarWidget>(PC, widgetClass);
		if (FloatingInfosBarWidget)
		{
			SetWidget(FloatingInfosBarWidget);
		}
	}
}

void UEnsFloatingInfosBarWidgetComponent::SetHealthPercentage(float healthPercentage) const
{
	FloatingInfosBarWidget->SetHealthPercentage(healthPercentage);
}
