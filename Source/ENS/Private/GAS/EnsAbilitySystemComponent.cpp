// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "GAS/EnsAbilitySystemComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

namespace
{
    /// \brief Returns the next usable input ID. All ids are unique within the program lifetime.
    [[nodiscard]] FAbilityInputBinding::Id NextInputId()
    {
        static FAbilityInputBinding::Id Id = FAbilityInputBinding::InvalidInputID;
        return ++Id;
    }
} // namespace

void UEnsAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();

    // Get a player state in the owner chain
    const ACharacter* PlayerCharacter = nullptr;
    for (UObject* Owner = GetOwner(); Owner; Owner = Owner->GetOuter())
    {
        if (const ACharacter* FoundPlayerController = Cast<ACharacter>(Owner))
        {
            PlayerCharacter = FoundPlayerController;
            break;
        }
    }

    if (!PlayerCharacter || !IsValid(PlayerCharacter))
    {
        UE_LOG(LogAbilitySystemComponent, Error, TEXT("No player character found in the owner chain. Cannot bind inputs."));
        return;
    }

    // Get the enhanced input component in the associated controller
    if (PlayerCharacter->InputComponent)
    {
        EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerCharacter->InputComponent);
        if (!EnhancedInputComponent)
            UE_LOG(LogAbilitySystemComponent, Error, TEXT("No enhanced input component found in the player controller. Cannot bind inputs."));
    }
    else
        UE_LOG(LogAbilitySystemComponent, Error, TEXT("No player controller found in the player state. Cannot bind inputs."));
}

void UEnsAbilitySystemComponent::SetInputBinding(UInputAction* InputAction, const FGameplayAbilitySpecHandle AbilityHandle)
{
    auto* AbilityBinding = MappedAbilities.Find(InputAction);
    if (AbilityBinding)
    {
        FGameplayAbilitySpec* OldBoundAbility = FindAbilitySpecFromHandle(AbilityBinding->BoundAbilitiesStack.Top());
        if (OldBoundAbility && OldBoundAbility->InputID == AbilityBinding->InputID)
            OldBoundAbility->InputID = FAbilityInputBinding::InvalidInputID;
    }
    else
    {
        AbilityBinding = &MappedAbilities.Add(InputAction);
        AbilityBinding->InputID = NextInputId();
    }

    if (auto* BindingAbility = FindAbilitySpecFromHandle(AbilityHandle))
        BindingAbility->InputID = AbilityBinding->InputID;

    AbilityBinding->BoundAbilitiesStack.Add(AbilityHandle);
    TryBindAbilityInput(InputAction, *AbilityBinding);
}

void UEnsAbilitySystemComponent::RemoveInputBinding(UInputAction* InputAction)
{
    if (!EnhancedInputComponent)
    {
        UE_LOG(LogAbilitySystemComponent, Warning, TEXT("No input component found. Cannot unbind input %s."), *InputAction->GetName());
        return;
    }

    if (FAbilityInputBinding* Bindings = MappedAbilities.Find(InputAction))
    {
        // Unbind the input
        EnhancedInputComponent->RemoveBindingByHandle(Bindings->OnPressedHandle);
        EnhancedInputComponent->RemoveBindingByHandle(Bindings->OnReleasedHandle);

        // Set invalid ids for all abilities bound to this input
        for (const FGameplayAbilitySpecHandle AbilityHandle : Bindings->BoundAbilitiesStack)
            if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilityHandle); AbilitySpec && AbilitySpec->InputID == Bindings->InputID)
                AbilitySpec->InputID = FAbilityInputBinding::InvalidInputID;

        MappedAbilities.Remove(InputAction);
    }
}

void UEnsAbilitySystemComponent::ClearBindingsForAbility(const FGameplayAbilitySpecHandle AbilityHandle)
{
    if (FGameplayAbilitySpec* FoundAbility = FindAbilitySpecFromHandle(AbilityHandle))
    {
        // Find the mapping for this ability => there should be a FindByPredicate() method in TMap, but there is none :(
        auto MappedIterator = MappedAbilities.CreateIterator();
        while (MappedIterator)
        {
            if (MappedIterator.Value().InputID == FoundAbility->InputID)
                break;
            ++MappedIterator;
        }

        if (MappedIterator)
            return;

        // Unbind all inputs for this ability
        FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();
        if (AbilityInputBinding.BoundAbilitiesStack.Remove(AbilityHandle) > 0)
        {
            if (AbilityInputBinding.BoundAbilitiesStack.Num() > 0)
            {
                FGameplayAbilitySpec* StackedAbility = FindAbilitySpecFromHandle(AbilityInputBinding.BoundAbilitiesStack.Top());
                if (StackedAbility && StackedAbility->InputID == 0)
                    StackedAbility->InputID = AbilityInputBinding.InputID;
            }
            else // This will invalidate the `AbilityInputBinding` ref above
                RemoveInputBinding(MappedIterator.Key());

            // `AbilityInputBinding` may be deleted from here
            FoundAbility->InputID = FAbilityInputBinding::InvalidInputID;
        }
    }
}

void UEnsAbilitySystemComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
    if (const FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction))
    {
        ensure(FoundBinding->InputID != FAbilityInputBinding::InvalidInputID);
        AbilityLocalInputPressed(FoundBinding->InputID);
    }
}

void UEnsAbilitySystemComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
    if (const FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction))
    {
        ensure(FoundBinding->InputID != FAbilityInputBinding::InvalidInputID);
        AbilityLocalInputReleased(FoundBinding->InputID);
    }
}

void UEnsAbilitySystemComponent::TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding)
{
    if (!EnhancedInputComponent)
    {
        UE_LOG(LogAbilitySystemComponent, Warning, TEXT("No input component found. Cannot bind input %s to ability."), *InputAction->GetName());
        return;
    }

    // Pressed event
    if (AbilityInputBinding.OnPressedHandle == 0)
        AbilityInputBinding.OnPressedHandle = EnhancedInputComponent->BindAction(InputAction,
                                                                                 ETriggerEvent::Started,
                                                                                 this,
                                                                                 &UEnsAbilitySystemComponent::OnAbilityInputPressed,
                                                                                 InputAction)
                                                  .GetHandle();

    // Released event
    if (AbilityInputBinding.OnReleasedHandle == 0)
        AbilityInputBinding.OnReleasedHandle = EnhancedInputComponent->BindAction(InputAction,
                                                                                  ETriggerEvent::Completed,
                                                                                  this,
                                                                                  &UEnsAbilitySystemComponent::OnAbilityInputReleased,
                                                                                  InputAction)
                                                   .GetHandle();
}
