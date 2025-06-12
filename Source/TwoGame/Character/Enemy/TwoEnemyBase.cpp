// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoEnemyBase.h"

ATwoEnemyBase::ATwoEnemyBase(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UTwoCMC>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTwoAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UTwoAttributeSetBase>(TEXT("AttributeSetBase"));
}

void ATwoEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATwoEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwoEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
