// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.


#include "Spawner/Spawner.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spawner/SpawnDataRow.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SpawnRange = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRange"));

}

void ASpawner::EnemyDestroyed()
{
    ennemyCount--;
    if(ennemyCount == 0)
    {
        Spawn();
        
    }
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
    Super::BeginPlay();
    // Spawn();
	
}

void ASpawner::SpawnEnemy(int32 number, TSubclassOf<class AEnsEnemyBase> enemyClass)
{
    for(int32 i = 0; i < number; ++i )
    {
        FVector pos = UKismetMathLibrary::RandomPointInBoundingBox_Box(SpawnRange->Bounds.GetBox());
        while(IsInFrustum(pos))
        {
            pos = UKismetMathLibrary::RandomPointInBoundingBox_Box(SpawnRange->Bounds.GetBox());
        }
        
        FRotator rotation(0.0f, 0.0f, 0.0f);
        FActorSpawnParameters SpawnInfo;
        AEnsEnemyBase* enemy = GetWorld()->SpawnActor<AEnsEnemyBase>(enemyClass, pos, rotation, SpawnInfo);
        enemy->OnEnemyDestroyDelegate.AddUniqueDynamic(this, &ASpawner::EnemyDestroyed);
    }
}
bool ASpawner::IsInFrustum( const FVector& point)
{
    const APlayerController* const PlayerController = Cast<const APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

    FVector2D ScreenLocation;
    PlayerController->ProjectWorldLocationToScreen(point, ScreenLocation);

    int32 ScreenWidth = 0;
    int32 ScreenHeight = 0;
    PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

    int32 ScreenX = (int32)ScreenLocation.X;
    int32 ScreenY = (int32)ScreenLocation.Y;

    return ScreenX >= 0 && ScreenY >= 0 && ScreenX < ScreenWidth && ScreenY < ScreenHeight;
}

void ASpawner::Spawn()
{
    int skull = 0;
    int imp = 0;
    int kam = 0;
    FSpawnDataRow* row =spawnData->FindRow<FSpawnDataRow>(FName(FString::FromInt(wave + 1 )),"");
    int32 c = 0;
    for(auto& r : row->mobs)
    {
        c += ennemies * r.percentage / 100;
        SpawnEnemy(ennemies * r.percentage / 100, r.enemyClass);
    }
    ennemyCount += skull + imp + kam;
    wave+= 1;
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

