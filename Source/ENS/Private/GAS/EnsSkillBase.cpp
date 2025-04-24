// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "GAS/EnsSkillBase.h"

float USkillBaseBlueprintLibrary::SkillDamageFromLevel(const TSubclassOf<UEnsSkillBase> Skill, uint8 Level)
{
    ensure(Skill != nullptr && "Skill is null");
    const auto Object = NewObject<UEnsSkillBase>(GetTransientPackage(), Skill->StaticClass());
    ensure(Object != nullptr && "Skill is not a subclass of UEnsSkillBase");

    const float Result = Object->Damage.GetValueAtLevel(Level);
    Object->ConditionalBeginDestroy();
    return Result;
}

TSubclassOf<AActor> USkillBaseBlueprintLibrary::SkillCollisionActorFromLevel(const TSubclassOf<UEnsSkillBase> Skill, const uint8 Level)
{
    ensure(Skill != nullptr && "Skill is null");
    const auto Object = NewObject<UEnsSkillBase>(GetTransientPackage(), Skill->StaticClass());
    ensure(Object != nullptr && "Skill is not a subclass of UEnsSkillBase");

    const TSubclassOf<AActor> Result = Object->CollisionActorPerLevel.IsValidIndex(Level) ? Object->CollisionActorPerLevel[Level] : nullptr;
    Object->ConditionalBeginDestroy();
    return Result;
}
