// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TwoInputDataAsset.generated.h"

//原生Action+Tag
USTRUCT(BlueprintType)
struct FGenshinInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};


/**
 * 绑定 InputAction 和 Tag
 */
UCLASS()
class TWOGAME_API UTwoInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//通过Tag获取 Action
	UFUNCTION(BlueprintPure)
	const UInputAction* GetInputActionFromTag(const FGameplayTag& InputTag) const;

	//通过Action获取 Tag
	UFUNCTION(BlueprintPure)
	FGameplayTag GetInputTagFromAction(const UInputAction* InputAction) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGenshinInputAction> AbilityInputActions;
};
