// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Containers/Map.h"
#include "CoreMinimal.h"

#include "SpawnDataRow.generated.h"

/**
 * \brief Data row of a data table containing data for spawning waves of enemies.
 */
USTRUCT()
struct ENS_API FSpawnDataRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    /// \brief Number of Imps to spawn.
    UPROPERTY(EditAnywhere)
    int32 Imps;

    /// \brief Number of Skull Kamikaze to spawn.
    UPROPERTY(EditAnywhere)
    int32 SkullKamikaze;

    /// \brief Number of Skull Fire to spawn.
    UPROPERTY(EditAnywhere)
    int32 SkullFire;

    /// \brief Duration of the wave in seconds.
    UPROPERTY(EditAnywhere, meta = (Units = "s"))
    int32 Time;
};
