// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Equipment/Inventory.h"
#include "Equipment/BaseArmor.h"
#include "Equipment/BaseWeapon.h"
#include "GAS/EnsGameplayAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogInventory);

UInventory::UInventory()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventory::BeginPlay()
{
    Super::BeginPlay();

    ensure(Weapons.Num() > 0);
    ensure(Weapons.Num() == Armors.Num());

    const APawn* Pawn = Cast<APawn>(GetOwner());
    if (!Pawn)
    {
        UE_LOG(LogInventory, Error, TEXT("Inventory component must be attached to a pawn."));
        return;
    }

    // Get the player controller
    const APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
    if (!PlayerController)
    {
        UE_LOG(LogInventory, Error, TEXT("Inventory component must be attached to a player controller."));
        return;
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
    {
        EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &UInventory::SwapEquipmentSet);
        EnhancedInputComponent->BindAction(MainAbilityAction, ETriggerEvent::Started, this, &UInventory::HandleMainAbility);
        EnhancedInputComponent->BindActionValueLambda(LightSetAction, ETriggerEvent::Started, [&](const FInputActionValue&) { HandleSet(0); });
        EnhancedInputComponent->BindActionValueLambda(MediumSetAction, ETriggerEvent::Started, [&](const FInputActionValue&) { HandleSet(1); });
        EnhancedInputComponent->BindActionValueLambda(HeavySetAction, ETriggerEvent::Started, [&](const FInputActionValue&) { HandleSet(2); });
    }

    // Initialize the equipment set (at index 0)
    UpdateEquippedSet();
}

void UInventory::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    SwapCooldownTimer += DeltaTime;
}

ABaseWeapon* UInventory::GetCurrentWeapon()
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

void UInventory::SwapEquipmentSet(const FInputActionValue& Value)
{
    HandleSet((CurrentEquipmentIndex + Weapons.Num() + (Value.Get<float>() > 0 ? 1 : -1)) % Weapons.Num());
}

void UInventory::UpdateEquippedSet()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    // Destroy previous weapon if it exists
    if (AttachedWeapon)
    {
        // Remove abilities using stored handles
        if (UAbilitySystemComponent* AbilitySystem = Character->FindComponentByClass<UAbilitySystemComponent>())
        {
            if (BaseAttackHandle.IsValid())
                AbilitySystem->ClearAbility(BaseAttackHandle);
            if (MainAbilityHandle.IsValid())
                AbilitySystem->ClearAbility(MainAbilityHandle);
        }
        Character->GetMesh()->MoveIgnoreActors.Remove(AttachedWeapon);
        Character->GetMesh()->MoveIgnoreActors.Remove(AttachedArmor);

        AttachedArmor->Destroy();
        AttachedWeapon->Destroy();

        AttachedWeapon = nullptr;
        AttachedArmor = nullptr;
    }

    // Spawn and attach new weapon
    if (auto& WeaponClass = Weapons[CurrentEquipmentIndex])
    {
        AttachedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
        bool bIsAttached = AttachedWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachedWeapon->SocketName);
        if (!bIsAttached)
            UE_LOG(LogInventory, Error, TEXT("Failed to attach weapon to the character mesh."));

        Character->GetMesh()->MoveIgnoreActors.Add(AttachedWeapon);
        if (UAbilitySystemComponent* AbilitySystem = Character->FindComponentByClass<UAbilitySystemComponent>())
        {
            ensure(AttachedWeapon->BaseAttack);
            ensure(AttachedWeapon->MainAbility);

            BaseAttackHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(AttachedWeapon->BaseAttack));
            MainAbilityHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(AttachedWeapon->MainAbility));
        }
    }

    if (auto& ArmorClass = Armors[CurrentEquipmentIndex])
    {
        AttachedArmor = GetWorld()->SpawnActor<ABaseArmor>(ArmorClass, FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
        const bool bIsAttached = AttachedArmor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachedArmor->SocketName);
        if (!bIsAttached)
            UE_LOG(LogInventory, Error, TEXT("Failed to attach armor to the character mesh."));

        Character->GetMesh()->MoveIgnoreActors.Add(AttachedArmor);
    }
}

void UInventory::HandleMainAbility()
{
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (UAbilitySystemComponent* AbilitySystem = Character->FindComponentByClass<UAbilitySystemComponent>())
    {
        AbilitySystem->TryActivateAbilityByClass(AttachedWeapon->MainAbility);
    }
}

void UInventory::HandleSet(const uint8_t SetIndex)
{
    if (SwapCooldownTimer <= SwapCooldown || CurrentEquipmentIndex == SetIndex || IsPlayerPlayingAnimation())
        return;

    SwapCooldownTimer = 0;
    CurrentEquipmentIndex = SetIndex;
    UpdateEquippedSet();
}

bool UInventory::IsPlayerPlayingAnimation()
{
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
        if (USkeletalMeshComponent* Mesh = Character->GetMesh())
            if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
                return AnimInstance->Montage_IsPlaying(nullptr);
    return false;
}
