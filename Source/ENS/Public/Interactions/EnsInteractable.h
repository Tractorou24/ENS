// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnsInteractable.generated.h"

/**
 * \brief Base class for any object that can be interacted with.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UEnsInteractable : public UInterface
{
    GENERATED_BODY()
};

/**
 * \brief Interface for any object that can be interacted with.
 */
class IEnsInteractable
{
    GENERATED_BODY()

public:
    /**
     * \brief Called when the object is interacted with.
     * \param SourceActor The actor that interacted with this object.
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    void Interact(AActor* SourceActor);
};
