// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "Interactions/EnsPickableObject.h"
#include "Characters/Player/EnsPlayerCharacter.h"
#include "Equipment/Inventory.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogPickableObject);

AEnsPickableObject::AEnsPickableObject()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(Mesh);
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnsPickableObject::OnOverlapBegin);
}

void AEnsPickableObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Give the object to the player
    const auto* Character = Cast<AEnsPlayerCharacter>(OtherActor);
    if (!Character)
        return;

    // Pick it and if failed, just return
    if (!Character->GetInventoryComponent()->PickItem(this))
        return;

    // Play the sound
    if (PickedSound)
        UGameplayStatics::PlaySoundAtLocation(this, PickedSound, GetActorLocation());
    else
        UE_LOG(LogPickableObject, Warning, TEXT("No sound set for the object %s"), *Name);

    // Destroy it
    Destroy();
}
