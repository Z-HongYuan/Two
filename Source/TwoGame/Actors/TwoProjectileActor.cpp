// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoProjectileActor.h"


ATwoProjectileActor::ATwoProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent  = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetUseCCD(true);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);
	ProjectileMesh->SetCollisionProfileName(FName("NoCollision"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetGenerateOverlapEvents(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ATwoProjectileActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATwoProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
