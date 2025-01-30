// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UBoxComponent;

UCLASS()
class ENS_API ASpawner : public AActor
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    ASpawner();
    UFUNCTION()
    void EnemyDestroyed();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    void SpawnEnemy(int32 number, TSubclassOf<class AEnsEnemyBase> enemyClass);

public:
    UPROPERTY(EditAnywhere)
    UBoxComponent* SpawnRange;
    void Spawn();
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    UPROPERTY(EditAnywhere)
    UDataTable* spawnData;
    float cd = 10;
    float timer = 0;
    int ennemies = 20;
    int wave = 1;

};
