// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "BaseArmor.generated.h"

/**
 * \brief Base class for any armor contained in the inventory and be attached to a character.
 */
UCLASS()
class ENS_API ABaseArmor : public AActor
{
    GENERATED_BODY()

public:
    explicit ABaseArmor();

    /// \brief Component representing the armor's model.
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    /// \brief Name of the socket where the armor should be attached.
    UPROPERTY(EditAnywhere)
    FName SocketName;
};
