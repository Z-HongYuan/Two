// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoWeaponBase.h"


ATwoWeaponBase::ATwoWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
}

void ATwoWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATwoWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
