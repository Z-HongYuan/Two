// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TwoInputDataAsset.h"
#include "TwoEnhancedInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOGAME_API UTwoEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UTwoEnhancedInputComponent();

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityAction(const UTwoInputDataAsset* InputDataAsset,
	                       UserClass* Object,
	                       PressedFuncType PressedFunc,
	                       ReleasedFuncType ReleasedFunc,
	                       HeldFuncType HeldFunc);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UTwoEnhancedInputComponent::BindAbilityAction(const UTwoInputDataAsset* InputDataAsset,
                                                   UserClass* Object,
                                                   PressedFuncType PressedFunc,
                                                   ReleasedFuncType ReleasedFunc,
                                                   HeldFuncType HeldFunc)
{
	check(InputDataAsset);
	for (const FGenshinInputAction& ActionData : InputDataAsset->AbilityInputActions)
	{
		if (ActionData.InputAction && ActionData.InputTag.IsValid())
		{
			if (HeldFunc)
			{
				BindAction(ActionData.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, ActionData.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(ActionData.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, ActionData.InputTag);
			}
			if (PressedFunc)
			{
				BindAction(ActionData.InputAction, ETriggerEvent::Started, Object, PressedFunc, ActionData.InputTag);
			}
		}
	}
}
