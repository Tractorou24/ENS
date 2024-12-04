// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "EnsCharacterBase.generated.h"
#include "GameFramework/Character.h"

/**
 * \brief The base character in the game that can have abilities.
 *
 * It is used for playable characters and mobs.
 */
UCLASS()
class ENS_API AEnsCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    /// \brief Constructs a character and setups the GAS with default attributes.
    explicit AEnsCharacterBase();

    /// \brief Called when the character dies.
    virtual void Death();

    /// \brief Gets the ability system component stored the character.
    UFUNCTION(BlueprintCallable)
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    /// \brief Called at the game start to initialize the character.
    virtual void BeginPlay() override;

    /// \brief Adds the startup effects to the character. (e.g. set attributes to default)
    virtual void AddStartupEffects();

protected:
    /// \brief The ability system component of the character using this state.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
    class UEnsAbilitySystemComponent* AbilitySystemComponent;

    // \brief The effects to apply when the character spawns.
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
    TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

    /// \brief The current attribute set of the character using this state.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
    class UEnsHealthAttributeSet* HealthAttributeSet;
};
