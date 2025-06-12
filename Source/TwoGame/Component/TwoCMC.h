// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TwoCMC.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOGAME_API UTwoCMC : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UTwoCMC();

protected:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
