// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"

#include "Inventory.generated.h"


struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENS_API UInventory : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventory();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /// \brief Cooldown between 2 weapon swap
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SwapCooldown = 0.5f;
    
    /// \brief Array of weapon classes available in the inventory
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<class ABaseWeapon>> Weapons;

    /// \brief Array of armor classes available in the inventory
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TArray<TSubclassOf<class ABaseArmor>> Armors;
    
    /// \brief Input action for weapon scrolling/switching
    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite)
    class UInputAction* ScrollAction;

    /// \brief Input action for triggering the weapon's main ability
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputAction* MainAbilityAction;

    /// \brief Input action for switching to 1st weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputAction* Weapon1Action;

    /// \brief Input action for switching to 2nd weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputAction* Weapon2Action;

    /// \brief Input action for switching to 3rd weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputAction* Weapon3Action;

    /// \brief Get the currently attached weapon
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    class ABaseWeapon* GetCurrentWeapon();

    /// \brief Get Spec Handle for base attack of current weapon
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    FGameplayAbilitySpecHandle& GetBaseAttackSpecHandle();

    /// \brief Get Spec Handle for main ability of current weapon
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    FGameplayAbilitySpecHandle& GetMainAbilitySpecHandle();

private:

    /// \brief Index of the currently equipped weapon in the Weapons array
    int8 CurrentEquipmentIndex = 0;

    /// \brief Timer for swap cooldown
    float SwapCooldownTimer = 0.0f;

    /// \brief Handles weapon switching logic when scroll input is received
    void SwapWeapon(const FInputActionValue& Value);

    /// \brief Currently attached weapon instance
    class ABaseWeapon* AttachedWeapon;

    /// \brief Currently attached armor instance
    class ABaseArmor* AttachedArmor;

    /// \brief Handle for the basic attack ability of the current weapon
    FGameplayAbilitySpecHandle BaseAttackHandle;

    /// \brief Handle for the main ability of the current weapon
    FGameplayAbilitySpecHandle MainAbilityHandle;
    
    /// \brief Updates the currently equipped weapon
    void UpdateEquippedWeapon();
    
    /// \brief Handles the main ability input event
    void HandleMainAbility();

    /// \brief Handles weapon switch to first weapon input event
    void HandleWeapon1();

    /// \brief Handles weapon switch to second weapon input event
    void HandleWeapon2();

    /// \brief Handles weapon switch to third weapon input event
    void HandleWeapon3();

    /**
     * \brief Checks if player is currently playing an animation
     * \return Whether an animation is playing
     */
    bool IsPlayerPlayingAnimation();
};
