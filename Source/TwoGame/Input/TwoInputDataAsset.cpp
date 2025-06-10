// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoInputDataAsset.h"

const UInputAction* UTwoInputDataAsset::GetInputActionFromTag(const FGameplayTag& InputTag) const
{
	for (FGenshinInputAction AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputTag == InputTag)
		{
			return AbilityInputAction.InputAction;
		}
	}
	return nullptr;
}

FGameplayTag UTwoInputDataAsset::GetInputTagFromAction(const UInputAction* InputAction) const
{
	for (FGenshinInputAction AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputAction == InputAction)
		{
			return AbilityInputAction.InputTag;
		}
	}
	return FGameplayTag();
}
