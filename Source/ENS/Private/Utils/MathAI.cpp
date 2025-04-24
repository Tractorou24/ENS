// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.


#include "Utils/MathAI.h"

#include "Kismet/KismetMathLibrary.h"

void UMathAI::GetProjectilePositionByTime(const float Time, const float StartPitchAngle, const float StartYawDirection, const float Speed, const float Gravity, FVector& Result)
{
    const float CosJumpAngle = UKismetMathLibrary::DegCos(StartPitchAngle);
    const float SinJumpAngle = UKismetMathLibrary::DegSin(StartPitchAngle);
    const float CosStartYawDirection = UKismetMathLibrary::DegCos(StartYawDirection);
    const float SinStartYawDirection = UKismetMathLibrary::DegSin(StartYawDirection);

    Result.X =Speed * CosJumpAngle * CosStartYawDirection * Time;
    Result.Y =Speed * CosJumpAngle * SinStartYawDirection * Time;
    Result.Z =Speed * SinJumpAngle * Time - Gravity * 0.5f * Time * Time;
}

float UMathAI::GetProjectileCustomGravity(const float JumpAngle, const float JumpSpeed, const float XYLength)
{
    return UKismetMathLibrary::DegSin(JumpAngle * 2) * JumpSpeed * JumpSpeed / XYLength;
}

void UMathAI::GetNextPointOnCircleByAngle(const FVector& CircleCenterPosition, const FVector& LastPosition, const float LinearSpeed, const float DeltaTime, const bool MoveClockwise, FVector& Result)
{
    const float CircleRadius = FVector::Dist2D(CircleCenterPosition, LastPosition);

    //Get current angle from X axis
    const float CurrentAngle = UKismetMathLibrary::Atan2(
        LastPosition.Y - CircleCenterPosition.Y, 
        LastPosition.X - CircleCenterPosition.X);

    //Get Angular displacement
    float AngleDisplacement = LinearSpeed*DeltaTime/CircleRadius;

    if (MoveClockwise)
        AngleDisplacement = -AngleDisplacement;

    const float NewAngle = CurrentAngle+AngleDisplacement;
    
    Result.X = CircleCenterPosition.X + UKismetMathLibrary::Cos(NewAngle) * CircleRadius;
    Result.Y = CircleCenterPosition.Y + UKismetMathLibrary::Sin(NewAngle) * CircleRadius;
    Result.Z = LastPosition.Z;
}
