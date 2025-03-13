// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Characters/EnsCharacterBase.h"
#include "GAS/AttributeSets/EnsHealthAttributeSet.h"
#include "GAS/EnsAbilitySystemComponent.h"

void AEnsCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    AddStartupEffects();
}

AEnsCharacterBase::AEnsCharacterBase()
{
    // Init GAS (creating the attribute set as sub-object auto-registers it with the AbilitySystemComponent)
    AbilitySystemComponent = CreateDefaultSubobject<UEnsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    HealthAttributeSet = CreateDefaultSubobject<UEnsHealthAttributeSet>(TEXT("HealthAttributeSet"));
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute())
        .AddUObject(this, &AEnsCharacterBase::HealthChanged);
}

void AEnsCharacterBase::OnDeath()
{
    // Reset all attributes
    AddStartupEffects();
}

UAbilitySystemComponent* AEnsCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AEnsCharacterBase::AddStartupEffects()
{
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is nullptr in AEnsCharacterBase::AddStartupEffects(). Cannot initialize effects."));
        return;
    }

    FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    for (const TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
    {
        FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0, EffectContext);
        if (NewHandle.IsValid())
            AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
    }
}
