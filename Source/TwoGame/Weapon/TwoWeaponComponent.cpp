// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoWeaponComponent.h"


UTwoWeaponComponent::UTwoWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTwoWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UTwoWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
