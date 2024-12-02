// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnsEnemyBase.h"
#include "Characters/EnsCharacterBase.h"
#include "TrainingDummy.generated.h"

UCLASS()
class ENS_API ATrainingDummy : public AEnsEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATrainingDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
