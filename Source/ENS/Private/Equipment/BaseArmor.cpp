// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "Equipment/BaseArmor.h"

ABaseArmor::ABaseArmor()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}
