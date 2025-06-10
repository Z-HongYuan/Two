// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TwoAbilitySystemComponentBase.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOGAME_API UTwoAbilitySystemComponentBase : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTwoAbilitySystemComponentBase();

	// 对传入的Spec添加InputTag后给予技能,这并不是安全的, 因为Spec不是const,需要修改Ability
	FGameplayAbilitySpecHandle GiveAbilityWithTag(FGameplayAbilitySpec& Spec);

	// 对传入的Spec添加InputTag后给予技能,蓝图方法
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Abilities",
		meta = (DisplayName = "Give Ability With Tag", ScriptName = "GiveAbilityWithTag"))
	FGameplayAbilitySpecHandle K2_GiveAbilityWithTag(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 0, int32 InputID = -1);

	/*
	 * 包括Pressed, Held, Released
	 * Pressed:尝试调用一次技能并且通知技能按下
	 * Held:尝试每帧调用一次技能并且通知技能按下
	 * Released:通知技能释放
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
