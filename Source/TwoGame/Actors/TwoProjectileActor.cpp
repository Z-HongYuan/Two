// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoProjectileActor.h"


ATwoProjectileActor::ATwoProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void ATwoProjectileActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATwoProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
