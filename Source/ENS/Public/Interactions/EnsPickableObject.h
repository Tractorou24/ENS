// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "EnsPickableObject.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPickableObject, Log, All);

/**
 * \brief An object that can be picked by the player.
 */
UCLASS()
class ENS_API AEnsPickableObject : public AActor
{
    GENERATED_BODY()

public:
    explicit AEnsPickableObject();

    /// \brief Gets the name of the object.
    [[nodiscard]] FString GetObjectName() const { return Name; }

    /// \brief Checks if the object is stackable.
    [[nodiscard]] bool IsStackable() const { return bIsStackable; }

    /**
     * \brief Called when the object is picked by the player. (it steps into \ref CollisionBox)
     * \param OverlappedComponent The component that overlapped with the object.
     * \param OtherActor The actor that overlapped with the object.
     * \param OtherComp The component of the actor that overlapped with the object.
     * \param OtherBodyIndex The index of the body that overlapped with the object.
     * \param bFromSweep `true` if the overlap is a result of a sweep.
     * \param SweepResult The result of the sweep.
     */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
    /// \brief The name of the object
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickable Object")
    FString Name = "Unnamed Object 0";

    /// \brief `true` if we can have multiple items of this type in the inventory, `false` otherwise
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickable Object")
    bool bIsStackable = true;

    /// \brief The mesh of the object
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Pickable Object")
    class UStaticMeshComponent* Mesh;

    /// \brief The sound played when the object is picked
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickable Object")
    TObjectPtr<USoundBase> PickedSound;

    /// \brief The collision box of the object
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Pickable Object")
    class UBoxComponent* CollisionBox;
};
