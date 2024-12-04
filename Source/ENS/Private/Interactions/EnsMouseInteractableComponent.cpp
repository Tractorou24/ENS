// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Interactions/EnsMouseInteractableComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogInteractions)

UEnsMouseInteractableComponent::UEnsMouseInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UEnsMouseInteractableComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Check if the object is below the mouse cursor
    const auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (FHitResult Hit; PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit))
    {
        if (Hit.GetActor() == GetOwner())
        {
            if (!bIsMouseHovering)
            {
                bIsMouseHovering = true;
                OnMouseHoverStart.Broadcast();
            }
        }
        else if (bIsMouseHovering)
        {
            bIsMouseHovering = false;
            OnMouseHoverEnd.Broadcast();
        }
    }
}

void UEnsMouseInteractableComponent::Interact_Implementation(AActor* SourceActor)
{
    IEnsInteractable::Interact_Implementation(SourceActor);
    OnInteract.Broadcast(SourceActor);
}

bool UEnsMouseInteractableComponent::IsMouseHovering() const
{
    return bIsMouseHovering;
}

bool UEnsMouseInteractableComponent::IsPlayerInZone() const
{
    return bIsPlayerInZone;
}

void UEnsMouseInteractableComponent::SetupInteractZone(UBoxComponent* BoxComponent)
{
    InteractZone = BoxComponent;
    InteractZone->OnComponentBeginOverlap.AddDynamic(this, &UEnsMouseInteractableComponent::OnBeginOverlap);
    InteractZone->OnComponentEndOverlap.AddDynamic(this, &UEnsMouseInteractableComponent::OnEndOverlap);
}

void UEnsMouseInteractableComponent::OnBeginOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*,
                                                    int32, bool, const FHitResult&)
{
    // If it is the player, it is in zone
    if (OtherActor == Cast<const AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) && !bIsPlayerInZone)
    {
        bIsPlayerInZone = true;
        OnPlayerEnterZone.Broadcast();
    }
    else if (bIsPlayerInZone)
        UE_LOG(LogInteractions, Error, TEXT("Interactable component of actor %s (BeginOverlap) is in an invalid state."),
               *GetOwner()->GetName())
}

void UEnsMouseInteractableComponent::OnEndOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32)
{
    if (OtherActor == Cast<const AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) && bIsPlayerInZone)
    {
        bIsPlayerInZone = false;
        OnPlayerExitZone.Broadcast();
    }
    else if (!bIsPlayerInZone)
        UE_LOG(LogInteractions, Error, TEXT("Intractable component of actor %s (EndOverlap) is in an invalid state."),
               *GetOwner()->GetName())
}
