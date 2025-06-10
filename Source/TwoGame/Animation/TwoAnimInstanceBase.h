// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "TwoAnimInstanceBase.generated.h"

/**
 * 带有Tag绑定的AnimInstance
 */
UCLASS()
class TWOGAME_API UTwoAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	//绑定bool和Tag的函数
	UFUNCTION(BlueprintCallable, Category = "GameplayTags")
	void SetGameplayTagPropertyMap(AActor* ListenInActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
