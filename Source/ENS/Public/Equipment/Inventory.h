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

public:

	/// \brief Array of weapon classes available in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class ABaseWeapon>> Weapons;

	/// \brief Input action for weapon scrolling/switching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite)
	class UInputAction* ScrollAction;

	/// \brief Input action for triggering the weapon's main ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* MainAbilityAction;

private:

	/// \brief Index of the currently equipped weapon in the Weapons array
	int8 CurrentWeapon = 0;

	/// \brief Handles weapon switching logic when scroll input is received
	void SwapWeapon(const FInputActionValue& Value);

	/// \brief Currently attached weapon instance
	class ABaseWeapon* AttachedWeapon;

	/// \brief Handle for the basic attack ability of the current weapon
	FGameplayAbilitySpecHandle BaseAttackHandle;

	/// \brief Handle for the main ability of the current weapon
	FGameplayAbilitySpecHandle MainAbilityHandle;
	
	/// \brief Updates the currently equipped weapon
	void UpdateEquippedWeapon();
	
	/// \brief Handles the main ability input event
	void HandleMainAbility(const FInputActionValue& InputActionValue);
};
