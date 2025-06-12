// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoWeaponBase.h"
#include "TwoGame/AbilitySystem/TwoAbilitySystemBlueprintLibrary.h"


ATwoWeaponBase::ATwoWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

void ATwoWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATwoWeaponBase::GiveWeaponAbilities()
{
	if (HasAuthority())
	{
		for (TSubclassOf<UGameplayAbility>& Ability : WeaponAbilities)
		{
			if (!Ability) continue;
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, -1, -1, this);
			AbilitySystem->GiveAbilityWithTag(AbilitySpec);
		}
	}
}

void ATwoWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
