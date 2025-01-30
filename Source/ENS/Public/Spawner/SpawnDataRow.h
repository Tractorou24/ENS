// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/EnsEnemyBase.h"
#include "SpawnDataRow.generated.h"

/**
 * 
 */
USTRUCT()
struct ENS_API FSpawnDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<AEnsEnemyBase> IA_1 = nullptr;

    UPROPERTY(EditAnywhere)
    int p_1 = 40;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AEnsEnemyBase> IA_2 = nullptr;

    UPROPERTY(EditAnywhere)
    int p_2 = 30;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AEnsEnemyBase> IA_3 = nullptr;

    UPROPERTY(EditAnywhere)
    int p_3 = 30;

    
};
