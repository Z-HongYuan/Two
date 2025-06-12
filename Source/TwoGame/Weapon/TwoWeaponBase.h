// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwoGame/AbilitySystem/TwoAbilitySystemComponentBase.h"
#include "TwoGame/AbilitySystem/Ability/TwoGameplayAbilityBase.h"
#include "TwoWeaponBase.generated.h"

/*
 * 如果装备此武器的Pawn有ASC,则将此武器的技能添加到Pawn的ASC中
 */
UCLASS()
class TWOGAME_API ATwoWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ATwoWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetOwnerPawn(AActor* InOwnerPawn) { OwnerPawn = InOwnerPawn; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UTwoAbilitySystemComponentBase> AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY()
	TObjectPtr<AActor> OwnerPawn;

	UFUNCTION()
	void GiveWeaponAbilities();
public:
	virtual void Tick(float DeltaTime) override;
};
