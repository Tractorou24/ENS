// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MathAI.generated.h"

/**
 * 
 */
UCLASS()
class ENS_API UMathAI : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    /**
     * \brief Returns a new point on a projectile path by the time
     * \param Time Elapsed time from the start
     * \param StartPitchAngle Pitch angle at the start
     * \param StartYawDirection Direction Yaw value at the start
     * \param Speed Projectile Speed at the start
     * \param Gravity Gravity force to apply to the projectile
     * \param Result The variable to put the result
     */
    UFUNCTION(BlueprintCallable, Category="Movement")
    static void GetProjectilePositionByTime(const float Time, const float StartPitchAngle, const float StartYawDirection, const float Speed, const float Gravity, FVector& Result);

    /**
     * \brief Get a custom gravity force so the projectile path will always have the XYLength for the given pitch angle and speed
     * \param StartPitchAngle Pitch angle at the start
     * \param Speed Projectile Speed at the start
     * \param XYLength Horizontal (x,y) length between start and end of the projectile path
     * \return The gravity force to apply
     */
    UFUNCTION(BlueprintCallable, Category="Movement")
    static float GetProjectileCustomGravity(const float StartPitchAngle, const float Speed, const float XYLength);

    /**
     * \brief Returns a new point on a circle moved at given speed from an start around the circle center
     * \param CircleCenterPosition Circle center 
     * \param LastPosition Starting point
     * \param LinearSpeed Movement speed on the circle
     * \param DeltaTime DeltaTime
     * \param MoveClockwise Decide if movement has to be clockwise or not
     * \param Result The variable to put the result
     */
    UFUNCTION(BlueprintCallable, Category="Movement")
    static void GetNextPointOnCircleByAngle(const FVector& CircleCenterPosition, const FVector& LastPosition, const float LinearSpeed, const float DeltaTime, const bool MoveClockwise, FVector& Result);
};
