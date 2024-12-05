// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.
#include "Equipment/BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}


