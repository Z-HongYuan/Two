// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "../TwoCharacterBase.h"
#include "TwoEnemyBase.generated.h"

/*
 * 敌人的基类
 * 继承父类的ASC和CoverMesh
 */
UCLASS()
class TWOGAME_API ATwoEnemyBase : public ATwoCharacterBase
{
	GENERATED_BODY()

public:
	ATwoEnemyBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
