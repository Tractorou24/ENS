// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "ENSGameMode.h"

#include "Characters/Player/EnsPlayerCharacter.h"
#include "Characters/Player/EnsPlayerController.h"

AEnsGameMode::AEnsGameMode()
{
    // Use custom PlayerController
    PlayerControllerClass = AEnsPlayerController::StaticClass();

    // Use blueprint character (which inherits from AEnsPlayerCharacter)
    DefaultPawnClass = AEnsPlayerCharacter::StaticClass();

    // Use blueprint controller (which inherits from AEnsPlayerController)
    PlayerControllerClass = AEnsPlayerController::StaticClass();
}
