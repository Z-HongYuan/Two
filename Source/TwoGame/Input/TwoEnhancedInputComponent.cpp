// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoEnhancedInputComponent.h"


UTwoEnhancedInputComponent::UTwoEnhancedInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTwoEnhancedInputComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UTwoEnhancedInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
