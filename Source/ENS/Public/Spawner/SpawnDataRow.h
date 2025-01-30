// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/EnsEnemyBase.h"
#include "Containers/Map.h"
#include "SpawnDataRow.generated.h"

USTRUCT()
struct FMobinfo
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<AEnsEnemyBase> enemyClass;
    UPROPERTY(EditAnywhere  )
    int32 percentage;
};
/**
 * 
 */
USTRUCT()
struct ENS_API FSpawnDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere)
    TArray<FMobinfo> mobs;


    
};
