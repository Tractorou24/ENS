// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "BaseArmor.generated.h"

UCLASS()
class ENS_API ABaseArmor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseArmor();

    /// \brief Static mesh component representing the weapon's 3D model
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    /// \brief Name of the socket where the armor should be attached
    UPROPERTY(EditAnywhere)
    FName SocketName;
};
