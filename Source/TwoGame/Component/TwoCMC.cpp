// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoCMC.h"
#include "TwoGame/Character/TwoCharacterBase.h"


UTwoCMC::UTwoCMC()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTwoCMC::BeginPlay()
{
	Super::BeginPlay();
}

float UTwoCMC::GetMaxSpeed() const
{
	// return Super::GetMaxSpeed();
	if (const ATwoCharacterBase* Character = Cast<ATwoCharacterBase>(GetOwner()))
	{
		return Character->GetAttributeSet()->GetMaxMoveSpeed();
	}

	return Super::GetMaxSpeed();
}


void UTwoCMC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
