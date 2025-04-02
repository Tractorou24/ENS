// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Interactions/EnsMouseInteractableComponent.h"

#include "Characters/Player/EnsPlayerController.h"
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
    if (FHitResult Hit; PlayerController->GetHitResultUnderCursor(ECustomCollisionChannel::ECC_Interactable, true, Hit))
    {
        if (Hit.GetComponent() == InteractClickZone)
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
    else if (bIsMouseHovering)
    {
        bIsMouseHovering = false;
        OnMouseHoverEnd.Broadcast();
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

void UEnsMouseInteractableComponent::SetupInteractZone(UBoxComponent* InteractionZone, UBoxComponent* ClickZone)
{
    InteractZone = InteractionZone;
    InteractZone->OnComponentBeginOverlap.AddDynamic(this, &UEnsMouseInteractableComponent::OnBeginOverlap);
    InteractZone->OnComponentEndOverlap.AddDynamic(this, &UEnsMouseInteractableComponent::OnEndOverlap);

    InteractClickZone = ClickZone;
    InteractClickZone->SetCollisionResponseToChannel(ECustomCollisionChannel::ECC_Interactable, ECR_Block);
}

void UEnsMouseInteractableComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                    int32, bool, const FHitResult&)
{
    auto* Character = Cast<const AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (OtherActor != Character)
        return;

    if (!bIsPlayerInZone)
    {
        bIsPlayerInZone = true;
        OnPlayerEnterZone.Broadcast();
    }
    else
        UE_LOG(LogInteractions, Error,
               TEXT("Interactable component of actor %s (BeginOverlap) is in an invalid state. Overlapped component: %s, Other component: %s"),
               *GetOwner()->GetName(), *OverlappedComponent->GetName(), *OtherComp->GetName())
}

void UEnsMouseInteractableComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCOmp, int32)
{
    auto* Character = Cast<const AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (OtherActor != Character)
        return;

    if (bIsPlayerInZone)
    {
        bIsPlayerInZone = false;
        OnPlayerExitZone.Broadcast();
    }
    else
        UE_LOG(LogInteractions, Error,
               TEXT("Intractable component of actor %s (EndOverlap) is in an invalid state. Overlapped component: %s, Other component: %s"),
               *GetOwner()->GetName(), *OverlappedComponent->GetName(), *OtherCOmp->GetName())
}
