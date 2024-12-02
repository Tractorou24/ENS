// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnsSpringArmComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpringArmComponent, Log, All);

/**
 * \brief Custom camera boom used by the \ref AEnsCharacter.
 *
 * It allows to move the camera when the mouse is on the edge of the viewport.
 * It can be configured using blueprint exposed variables.
 */
UCLASS()
class ENS_API UEnsSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	/**
	 * \brief Creates a new spring arm which moves the camera when the cursor is on the edge.
	 */
	explicit UEnsSpringArmComponent();

	/**
     * \brief Moves the camera depending on the cursor position. Called every frame.
     * \param DeltaTime Time since the last frame in seconds.
     * \param TickType Type of tick.
     * \param ThisTickFunction The tick function.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/// \brief The ratio [0; 1] of the viewport size (X & Y) to the mouse should pass to start moving the camera.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Configurable, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float LookAroundThreshold = 0.95;

	/// \brief The maximum offset the camera can reach in px.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Configurable, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
	int MaximumOffset = 150;

	/// \brief The speed at which the camera moves.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Configurable, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", UIMin = "0.0"))
	float MoveSpeed = 500;

	/// \brief The speed at which the camera returns to its original position.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Configurable, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", UIMin = "0.0"))
	float ReturnSpeed = 500;

	/// \brief The current offset of the camera relative to viewport center.
	FVector2D CurrentOffset = FVector2D::ZeroVector;
};
