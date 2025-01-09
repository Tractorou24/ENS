// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.


// Sets default values for this component's properties
#include "Equipment/Inventory.h"

#include <filesystem>

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Equipment/BaseArmor.h"
#include "Equipment/BaseWeapon.h"
#include "GameFramework/Character.h"
#include "GAS/EnsGameplayAbilityBase.h"

UInventory::UInventory()
{
    PrimaryComponentTick.bCanEverTick = true;
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
	        EnhancedInputComponent->BindAction(Weapon1Action, ETriggerEvent::Started, this, &UInventory::HandleWeapon1);
	        EnhancedInputComponent->BindAction(Weapon2Action, ETriggerEvent::Started, this, &UInventory::HandleWeapon2);
	        EnhancedInputComponent->BindAction(Weapon3Action, ETriggerEvent::Started, this, &UInventory::HandleWeapon3);
	    }
	}
    }
    UpdateEquippedWeapon();
}

void UInventory::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    SwapCooldownTimer += DeltaTime;
}

class ABaseWeapon* UInventory::GetCurrentWeapon()
{
    return AttachedWeapon;
}

FGameplayAbilitySpecHandle& UInventory::GetBaseAttackSpecHandle()
{
    return BaseAttackHandle;
}

FGameplayAbilitySpecHandle& UInventory::GetMainAbilitySpecHandle()
{
    return MainAbilityHandle;
}

void UInventory::SwapWeapon(const FInputActionValue& Value)
{
    if(SwapCooldownTimer <= SwapCooldown)
       return;
    if(IsPlayerPlayingAnimation())
        return;
    
    SwapCooldownTimer = 0;
    CurrentEquipmentIndex = (CurrentEquipmentIndex + Weapons.Num() +(Value.Get<float>() > 0 ? 1 : -1)) % Weapons.Num();
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
        Character->GetMesh()->MoveIgnoreActors.Remove(AttachedArmor);
        
        AttachedArmor->Destroy();
	AttachedWeapon->Destroy();
        
	AttachedWeapon = nullptr;
        AttachedArmor = nullptr;
    }
    
    // Spawn and attach new weapon
    if (Weapons[CurrentEquipmentIndex])
    {
	AttachedWeapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor<AActor>(Weapons[CurrentEquipmentIndex], FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator));
	AttachedWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachedWeapon->SocketName);
	Character->GetMesh()->MoveIgnoreActors.Add(AttachedWeapon);
        AttachedWeapon->WeaponInfo->
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
    if(Armors[CurrentEquipmentIndex])
    {
        AttachedArmor = Cast<ABaseArmor>(GetWorld()->SpawnActor<AActor>(Armors[CurrentEquipmentIndex], FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator));
        AttachedArmor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachedArmor->SocketName);
        Character->GetMesh()->MoveIgnoreActors.Add(AttachedArmor);
    }
}

void UInventory::HandleMainAbility()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !AttachedWeapon || !AttachedWeapon->MainAbility)
        return;

    if (UAbilitySystemComponent* AbilitySystem = Character->FindComponentByClass<UAbilitySystemComponent>())
    {
        AbilitySystem->TryActivateAbilityByClass(AttachedWeapon->MainAbility);
    }
}

void UInventory::HandleWeapon1()
{
    if(SwapCooldownTimer <= SwapCooldown || CurrentEquipmentIndex == 0)
        return;
    
    if(IsPlayerPlayingAnimation())
        return;
    
    SwapCooldownTimer = 0;
    CurrentEquipmentIndex = 0;
    UpdateEquippedWeapon();
}

void UInventory::HandleWeapon2()
{
    if(SwapCooldownTimer <= SwapCooldown || CurrentEquipmentIndex == 1)
        return;
    
    if(IsPlayerPlayingAnimation())
        return;
    
    SwapCooldownTimer = 0;
    CurrentEquipmentIndex = 1;
    UpdateEquippedWeapon();
}

void UInventory::HandleWeapon3()
{
    if(SwapCooldownTimer <= SwapCooldown || CurrentEquipmentIndex == 2)
        return;
    
    if(IsPlayerPlayingAnimation())
        return;
    
    SwapCooldownTimer = 0;
    CurrentEquipmentIndex = 2;
    UpdateEquippedWeapon();
}

bool UInventory::IsPlayerPlayingAnimation()
{
    if ( ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        if (USkeletalMeshComponent* Mesh = Character->GetMesh())
        {
            if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
            {
                return AnimInstance->Montage_IsPlaying(nullptr);
            }
        }
    }
    return false;
}   