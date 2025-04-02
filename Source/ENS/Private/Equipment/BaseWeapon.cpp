// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

#include "Equipment/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}
