// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.


#include "Spawner/Spawner.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spawner/SpawnDataRow.h"

#include <string>

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SpawnRange = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRange"));

}

void ASpawner::EnemyDestroyed() {}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
    Super::BeginPlay();
	
}

void ASpawner::SpawnEnemy(int32 number, TSubclassOf<class AEnsEnemyBase> enemyClass)
{
    for(int32 i = 0; i < number; ++i )
    {
        FVector pos = UKismetMathLibrary::RandomPointInBoundingBox_Box(SpawnRange->Bounds.GetBox());
        FRotator rotation(0.0f, 0.0f, 0.0f);
        FActorSpawnParameters SpawnInfo;
        AEnsEnemyBase* enemy = GetWorld()->SpawnActor<AEnsEnemyBase>(enemyClass, pos, rotation, SpawnInfo);
        enemy->OnEnemyDestroyDelegate.AddUniqueDynamic(this, &ASpawner::EnemyDestroyed);
    }
}

void ASpawner::Spawn()
{
    int skull = 0;
    int imp = 0;
    int kam = 0;
    FSpawnDataRow* row =spawnData->FindRow<FSpawnDataRow>(FName(FString::FromInt(wave)),"");
    if(row)
    {
        imp = ennemies * row->p_1 / 100;
        kam = ennemies * row->p_2 / 100;
        skull = ennemies * row->p_3 / 100;
        
        SpawnEnemy(imp, row->IA_1);
        SpawnEnemy(kam, row->IA_2);
        SpawnEnemy(skull, row->IA_3);
    }
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    timer += DeltaTime;
    if(timer >= cd)
    {
        timer = 0;
        Spawn();
    }
    

}

