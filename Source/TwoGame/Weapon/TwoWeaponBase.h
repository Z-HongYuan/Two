// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwoGame/AbilitySystem/TwoAbilitySystemComponentBase.h"
#include "TwoWeaponBase.generated.h"

UCLASS()
class TWOGAME_API ATwoWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ATwoWeaponBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UTwoAbilitySystemComponentBase> AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

public:
	virtual void Tick(float DeltaTime) override;
};
