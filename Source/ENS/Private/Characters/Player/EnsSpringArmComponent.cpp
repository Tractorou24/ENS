// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#include "Characters/Player/EnsSpringArmComponent.h"
#include "Characters/Player/EnsPlayerController.h"

DEFINE_LOG_CATEGORY(LogSpringArmComponent);

/**
 * \brief Applies camera return offset to given \p Value.
 * \param Value The value to offset.
 * \param ReturnSpeed The speed at which the camera returns to center.
 * \param DeltaTime The time since the last frame.
 */
void OffsetAxis(double& Value, const float ReturnSpeed, const float DeltaTime)
{
	if (Value != 0)
	{
		const float Direction = Value > 0 ? -1.0f : 1.0f;
		Value += Direction * ReturnSpeed * DeltaTime;
		if ((Direction < 0 && Value < 0) || (Direction > 0 && Value > 0))
			Value = 0;
	}
}

UEnsSpringArmComponent::UEnsSpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnsSpringArmComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector2D Mouse = FVector2D::ZeroVector;
	if (const auto* Controller = Cast<AEnsPlayerController>(GetOwner()->GetInstigatorController()))
		Mouse = Controller->GetRelativeMousePosition();
	else
    {
        UE_LOG(LogSpringArmComponent, Error, TEXT("Failed to get the player controller."));
        return;
    }

	// Move the camera to the edge
	FVector2D MaxOffset = {static_cast<double>(MaximumOffset), static_cast<double>(MaximumOffset)};
	if (Mouse.X > LookAroundThreshold || Mouse.X < 1 - LookAroundThreshold || Mouse.Y > LookAroundThreshold || Mouse.Y < 1 - LookAroundThreshold)
	{
		MaxOffset = {MaximumOffset * Mouse.X * 2 - MaximumOffset, MaximumOffset * Mouse.Y * 2 - MaximumOffset};
		const FVector2f Direction = {MaxOffset.X > 0 ? 1.0f : -1.0f, MaxOffset.Y > 0 ? -1.0f : 1.0f};
		CurrentOffset += {Direction.X * MoveSpeed * DeltaTime, Direction.Y * MoveSpeed * DeltaTime};

	} else // Return the camera to its original position
	{
		OffsetAxis(CurrentOffset.Y, ReturnSpeed, DeltaTime);
		OffsetAxis(CurrentOffset.X, ReturnSpeed, DeltaTime);
	}

	// Move the camera at the correct location depending on offset
	CurrentOffset.X = FMath::Clamp(CurrentOffset.X, -FMath::Abs(MaxOffset.X), FMath::Abs(MaxOffset.X));
	CurrentOffset.Y = FMath::Clamp(CurrentOffset.Y, -FMath::Abs(MaxOffset.Y), FMath::Abs(MaxOffset.Y));
	SetWorldLocation(GetOwner()->GetActorLocation() + FVector(CurrentOffset.Y, CurrentOffset.X, 0.0f));
}
