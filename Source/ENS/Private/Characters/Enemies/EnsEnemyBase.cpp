// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.


#include "Characters/Enemies/EnsEnemyBase.h"

#include "Components/BoxComponent.h"
#include "GAS/EnsAbilitySystemComponent.h"
#include "GAS/AttributeSets/EnsHealthAttributeSet.h"
#include "Interactions/EnsMouseInteractableComponent.h"
#include "UI/EnsFloatingInfosBarWidget.h"
#include "UI/EnsFloatingInfosBarWidgetComponent.h"

// Sets default values
AEnsEnemyBase::AEnsEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Add UI Component
	FloatingInfosBarWidgetComponent = CreateDefaultSubobject<UEnsFloatingInfosBarWidgetComponent>(
		FName("FloatingStatusBarComponent"));
	FloatingInfosBarWidgetComponent->SetupAttachment(RootComponent);
	FloatingInfosBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 120));
	FloatingInfosBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingInfosBarWidgetComponent->SetDrawSize(FVector2d(500, 500));

	InteractZone = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
    InteractZone->SetupAttachment(RootComponent);

	MouseInteractableComponent = CreateDefaultSubobject<UEnsMouseInteractableComponent>(TEXT("Interactions"));
	MouseInteractableComponent->SetupInteractZone(InteractZone);
}

// Called when the game starts or when spawned
void AEnsEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupEffects();

		FloatingInfosBarWidgetClass = StaticLoadClass(UObject::StaticClass(), nullptr,
		                                              TEXT(
			                                              "/Game/Developers/Fabian/WB_FloatingInfoBar.WB_FloatingInfoBar_C"));
		if (!FloatingInfosBarWidgetClass)
			UE_LOG(LogTemp, Error,
		       TEXT(
			       "%s() Failed to find WB_FloatingInfoBar. If it was moved, please update the reference location in C++."
		       ), *FString(__FUNCTION__));

		FloatingInfosBarWidgetComponent->AddWidget(FloatingInfosBarWidgetClass);
		FloatingInfosBarWidgetComponent->SetHealthPercentage(1.f);
	}

	// Attribute change callbacks
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                                                      HealthAttributeSet->GetHealthAttribute()).
	                                                      AddUObject(this, &AEnsEnemyBase::HealthChanged);
}

void AEnsEnemyBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	const float Health = Data.NewValue;

	// Update floating status bar
	if (FloatingInfosBarWidgetComponent)
		FloatingInfosBarWidgetComponent->SetHealthPercentage(Health / HealthAttributeSet->GetMaxHealth());

	// TODO: If died, handle death
}
