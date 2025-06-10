// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoAnimInstanceBase.h"
#include "AbilitySystemGlobals.h"

void UTwoAnimInstanceBase::SetGameplayTagPropertyMap(AActor* ListenInActor)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ListenInActor))
	{
		check(ASC);
		GameplayTagPropertyMap.Initialize(this, ASC);
	}
}
