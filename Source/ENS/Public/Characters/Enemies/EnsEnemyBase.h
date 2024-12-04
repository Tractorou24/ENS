// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Characters/EnsCharacterBase.h"
#include "CoreMinimal.h"
#include "EnsEnemyBase.generated.h"

UCLASS()
class ENS_API AEnsEnemyBase : public AEnsCharacterBase
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnsEnemyBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ens|UI")
    TSubclassOf<class UEnsFloatingInfosBarWidget> FloatingInfosBarWidgetClass;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ens|UI")
    class UEnsFloatingInfosBarWidgetComponent* FloatingInfosBarWidgetComponent;

    FDelegateHandle HealthChangedDelegateHandle;

    // Attribute changed callbacks
    virtual void HealthChanged(const struct FOnAttributeChangeData& Data);

private:
    UPROPERTY(EditAnywhere, Category = "Interactions", meta = (AllowPrivateAccess = "true"))
    class UEnsMouseInteractableComponent* MouseInteractableComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Interactions", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* InteractZone = nullptr;
};
