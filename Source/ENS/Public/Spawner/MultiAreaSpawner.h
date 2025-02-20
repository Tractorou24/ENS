// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "Characters/Enemies/EnsEnemyBase.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MultiAreaSpawner.generated.h"

class UBoxComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogSpawners, Log, All);

/**
 * \brief Distribution of enemies to spawn on a specific area.
 */
USTRUCT(BlueprintType)
struct FSpawnDistribution
{
    GENERATED_BODY()

    /// \brief Index of the spawn area. `-1` if not set/invalid.
    int32 Index = -1;

    /// \brief Number of Imps to spawn.
    UPROPERTY()
    int32 Imps = 0;

    /// \brief Number of Skull Fire to spawn.
    UPROPERTY()
    int32 SkullFire = 0;

    /// \brief Number of Skull Kamikaze to spawn.
    UPROPERTY()
    int32 SkullKamikaze = 0;

    /// \return The total number of enemies to spawn.
    int32 GetTotal() const { return Imps + SkullFire + SkullKamikaze; }
};

/**
 * \brief A spawner that can have multiple spawn areas and a trigger box to activate it.
 */
UCLASS()
class ENS_API AMultiAreaSpawner : public AActor
{
    GENERATED_BODY()

public:
    explicit AMultiAreaSpawner();

    /**
     * \brief Get a random location within the bounds of the specified area.
     * \param AreaIndex Index of the area (must be in \ref SpawnAreas).
     * \return Location within the bounds of the specified area.
     */
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    FVector GetRandomLocationInSpecificArea(int32 AreaIndex) const;

    /**
     * \brief Callback function called when an enemy is destroyed.
     */
    UFUNCTION()
    void OnEnemyDestroyed();

    /// \brief `true` if the spawner is active, `false` otherwise.
    // TODO(Kevin): Merge with hasTrigger.
    UPROPERTY(EditAnywhere, Category = "Spawning")
    bool bIsActive;

    /// \brief Data table containing data for spawning waves of enemies.
    UPROPERTY(EditAnywhere, Category = "Spawning")
    UDataTable* SpawnData;

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Tick(float DeltaTime) override;

    /// \brief Display debug bounds in editor.
    UPROPERTY(EditAnywhere, Category = "Spawning")
    bool bShowDebugBounds = true;

#pragma region Spawn
    /// \brief Get indexes of active spawn areas. (i.e. areas that are further that \ref SpawnDistance from the player)
    [[nodiscard]] TArray<uint32> GetActiveSpawnAreas() const;

    /// \brief Number of spawn areas (minimum 1).
    UPROPERTY(EditAnywhere, Category = "Spawning", Meta = (ClampMin = "1"))
    int32 NumSpawnAreas = 1;

    /// \brief Default size of any box created automatically by the spawner (trigger box and spawn areas).
    UPROPERTY(EditAnywhere, Category = "Spawning")
    FVector SpawnAreaSize = FVector(200.0f, 200.0f, 200.0f);

    /// \brief Distance from the player for a spawn area to be deactivated. (i.e. if the player is closer than this distance, the area is not active)
    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnDistance = 500.0f;

    /// \brief Array containing all box components representing spawn areas of the spawner.
    UPROPERTY(BlueprintReadOnly, Category = "Spawning")
    TArray<UBoxComponent*> SpawnAreas;
#pragma endregion

#pragma region TriggerBox
    /// \brief Whether the Spawner has a trigger box or not to activate it.
    UPROPERTY(EditAnywhere, Category = "Spawning")
    bool bHasTrigger;

    /// \brief Trigger box used to activate the spawner.
    UPROPERTY(BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* TriggerBox;
#pragma endregion

    /**
     * \brief Whether the spawner should spawn waves of enemies infinitely or not.
     *
     * If `true`, the spawner will keep spawning waves of enemies indefinitely, restarting from wave 0 after the last wave.
     * If `false`, the spawner will stop spawning enemies after the last wave.
     */
    UPROPERTY(EditAnywhere, Category = "Spawning")
    bool bIsInfinite = false;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    TSubclassOf<AEnsEnemyBase> ImpClass;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    TSubclassOf<AEnsEnemyBase> SkullKamikazeClass;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    TSubclassOf<AEnsEnemyBase> SkullFireClass;

private:
    UFUNCTION()
    void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /// \brief Update the spawn areas of the spawner.
    void UpdateSpawnAreas();

    /// \brief Update the trigger box of the spawner.
    void UpdateTrigger();

    /**
     * \brief Distributes a number of specified enemies across all different active spawn areas.
     * \param Imps Number of imps.
     * \param SkullFire Number of Skull Fire.
     * \param SkullKamikaze Number of Skull Kamikaze.
     * \return Distribution of enemies to spawn across the active spawning areas.
     */
    TArray<FSpawnDistribution> DistributeEnemies(int32 Imps, int32 SkullFire, int32 SkullKamikaze);

    /// \brief Spawns the wave of enemies specified by `WaveNumber + 1`.
    void SpawnWave();

    /**
     * \brief Spawn a specific number of enemies inside the specified spawn area.
     * \param ActorToSpawn Class of the enemy to spawn.
     * \param AreaIndex Index of the spawn area.
     * \param Count Number of enemies to spawn.
     */
    void SpawnEnemies(const TSubclassOf<AEnsEnemyBase>& ActorToSpawn, int32 AreaIndex, int32 Count);

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    /// \brief Timer until the next wave of enemies is spawned.
    float Timer = 1000.0f;

    uint16 WaveNumber = 0;
    uint32 CurrentEnemies = 0;
};
