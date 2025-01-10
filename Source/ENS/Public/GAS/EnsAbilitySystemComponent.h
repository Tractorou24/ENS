// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"

#include "EnsAbilitySystemComponent.generated.h"

class UInputAction;

/**
 * \brief Link between an ability and an input action.
 */
USTRUCT()
struct FAbilityInputBinding
{
    GENERATED_BODY()

public:
    /// \brief The type of the ID used to identify the input.
    using Id = std::size_t;

    /// \brief The ID used to identify an invalid input (not bound).
    static constexpr Id InvalidInputID = 0;

public:
    /// \brief A custom ID representing the input. (unique)
    Id InputID = 0;

    /// \brief Handle returned by the BindAction function when the input is pressed.
    unsigned OnPressedHandle = 0;

    /// \brief Handle returned by the BindAction function when the input is released.
    unsigned OnReleasedHandle = 0;

    /// \brief All the abilities that are bound to this input.
    TArray<FGameplayAbilitySpecHandle> BoundAbilitiesStack;
};

/**
 * \brief The default ability system component for any object using GAS.
 */
UCLASS()
class ENS_API UEnsAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    /// \brief Initializes the component. Required since it needs to find the \ref EnhancedInputComponent in the owner chain.
    virtual void BeginPlay() override;

    /**
     * \brief Binds the given \p InputAction to the given \p AbilityHandle.
     * \param InputAction The input action to bind.
     * \param AbilityHandle The ability that will be triggered when \p InputAction is triggered.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SetInputBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle);

    /**
     * \brief Removes the binding between the given \p InputAction and all the abilities it triggers.
     * \param InputAction The input action to unbind.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void RemoveInputBinding(UInputAction* InputAction);

    /**
     * \brief Removes the binding between the given \p AbilityHandle and all the input actions that triggers it.
     * \param AbilityHandle The ability to unbind.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ClearBindingsForAbility(FGameplayAbilitySpecHandle AbilityHandle);

private:
    /**
     * \brief Tries to bind the given \p InputAction to the given \p AbilityInputBinding.
     * \param InputAction The input action to bind.
     * \param AbilityInputBinding The binding to use.
     */
    void TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding);

    /// \brief Called when \p InputAction is pressed.
    void OnAbilityInputPressed(UInputAction* InputAction);

    /// \brief Called when \p InputAction is released.
    void OnAbilityInputReleased(UInputAction* InputAction);

private:
    /// \brief Link between input action and it's triggered abilities.
    UPROPERTY(Transient)
    TMap<UInputAction*, FAbilityInputBinding> MappedAbilities;

    /// \brief The enhanced input component.
    UPROPERTY(Transient)
    UEnhancedInputComponent* EnhancedInputComponent;
};
