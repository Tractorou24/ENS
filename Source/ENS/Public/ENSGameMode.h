// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ENSGameMode.generated.h"
#include "GameFramework/GameMode.h"

/**
 * \brief Basic, default game mode for the project
 */
UCLASS()
class ENS_API AEnsGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    /// \brief Sets up the game mode default classes.
    explicit AEnsGameMode();
};
