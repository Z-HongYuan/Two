// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TwoWeaponBase.h"
#include "Components/ActorComponent.h"
#include "TwoWeaponComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOGAME_API UTwoWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTwoWeaponComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ATwoWeaponBase>> WeaponArray;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
