// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "EnsAttributeSetBase.h"

#include "EnsMovementAttributeSet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMovementAttributeSet, Log, All);

class UEnsMovementAttributeSet;

/**
 * \brief An attribute data for the max speed, which syncs with `GetCharacterMovement()->MaxWalkSpeed`.
 *
 * This allows to set the max speed of the character using the GAS and sync it with the character movement component.
 * The initial value is set to the default value of the character movement component. (from \ref UEnsCharacterBase::BeginPlay()), then synced to the GAS value.
 *
 * \warning Only the GAS value must be used to change the speed at runtime.
 */
USTRUCT()
struct FMaxSpeedAttributeData : public FGameplayAttributeData
{
    GENERATED_BODY()

    /// Bring the base constructors (for unreal internal use)
    using FGameplayAttributeData::FGameplayAttributeData;

    /**
     * \brief Constructs a max speed attribute, which can be synced with the character movement component.
     * \param AttributeSet The attribute set owning this attribute used to get the component from owning actor.
     */
    explicit FMaxSpeedAttributeData(UEnsMovementAttributeSet* AttributeSet);

    /// \copydoc FGameplayAttributeData::SetCurrentValue()
    virtual void SetCurrentValue(float NewValue) override;

private:
    UPROPERTY(Transient)
    TWeakObjectPtr<UEnsMovementAttributeSet> OwningAttributeSet;
};

/**
 * \brief The attributes storage for any object having movement inside the GAS.
 */
UCLASS()
class ENS_API UEnsMovementAttributeSet : public UEnsAttributeSetBase
{
    GENERATED_BODY()

public:
    explicit UEnsMovementAttributeSet();

    /// \brief The current Speed
    UPROPERTY(BlueprintReadOnly, Category = "Speed")
    FMaxSpeedAttributeData MaxSpeed;
    ATTRIBUTE_ACCESSORS(UEnsMovementAttributeSet, MaxSpeed)
};
