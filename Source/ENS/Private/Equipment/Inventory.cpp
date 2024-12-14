// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.


// Sets default values for this component's properties
#include "Equipment/Inventory.h"

#include <filesystem>

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Equipment/BaseWeapon.h"
#include "GameFramework/Character.h"
#include "GAS/EnsGameplayAbilityBase.h"

UInventory::UInventory()
{

}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		// Get the player controller
		if (APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController()))
		{
			// Get the enhanced input component
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Bind the input action
				EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &UInventory::SwapWeapon);

				EnhancedInputComponent->BindAction(MainAbilityAction, ETriggerEvent::Started, this, &UInventory::HandleMainAbility);
            
			}
		}
	}
	UpdateEquippedWeapon();
	
}

void UInventory::SwapWeapon(const FInputActionValue& Value)
{
	CurrentWeapon = (CurrentWeapon + Weapons.Num() +(Value.Get<float>() > 0 ? 1 : -1)) % Weapons.Num();
    UpdateEquippedWeapon();
}

void UInventory::UpdateEquippedWeapon()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
		return;

	// Destroy previous weapon if it exists
	if (AttachedWeapon)
	{
		// Remove abilities using stored handles
		if (UAbilitySystemComponent* AbilitySystem = Character->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (BaseAttackHandle.IsValid())
			{
				AbilitySystem->ClearAbility(BaseAttackHandle);
			}
			if (MainAbilityHandle.IsValid())
			{
				AbilitySystem->ClearAbility(MainAbilityHandle);
			}
		}
		Character->GetMesh()->MoveIgnoreActors.Remove(AttachedWeapon);
		AttachedWeapon->Destroy();
		AttachedWeapon = nullptr;
	}
	
	// Spawn and attach new weapon
	if (Weapons[CurrentWeapon])
	{
		AttachedWeapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor<AActor>(Weapons[CurrentWeapon], FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator));
		AttachedWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachedWeapon->SocketName);
		Character->GetMesh()->MoveIgnoreActors.Add(AttachedWeapon);
		if (UAbilitySystemComponent* AbilitySystem = Character->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (AttachedWeapon->BaseAttack)
			{
				FGameplayAbilitySpec Spec(AttachedWeapon->BaseAttack);
				BaseAttackHandle = AbilitySystem->GiveAbility(Spec);
			}
			if (AttachedWeapon->MainAbility)
			{
				FGameplayAbilitySpec Spec(AttachedWeapon->MainAbility);
				MainAbilityHandle = AbilitySystem->GiveAbility(Spec);
			}
		}
	}
}

void UInventory::HandleMainAbility(const FInputActionValue& InputActionValue)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !AttachedWeapon || !AttachedWeapon->MainAbility)
        return;

    if (UAbilitySystemComponent* AbilitySystem = Character->FindComponentByClass<UAbilitySystemComponent>())
    {
        AbilitySystem->TryActivateAbilityByClass(AttachedWeapon->MainAbility);
    }
}