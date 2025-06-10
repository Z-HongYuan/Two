// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoAbilitySystemComponentBase.h"

#include "AbilitySystemLog.h"
#include "Ability/TwoGameplayAbilityBase.h"


UTwoAbilitySystemComponentBase::UTwoAbilitySystemComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	// SetIsReplicated(true); //选择设置是否复制
}

FGameplayAbilitySpecHandle UTwoAbilitySystemComponentBase::GiveAbilityWithTag(FGameplayAbilitySpec& Spec)
{
	if (const UTwoGameplayAbilityBase* GenshinAbility = Cast<UTwoGameplayAbilityBase>(Spec.Ability))
	{
		if (GenshinAbility->InputTag.IsValid())
		{
			Spec.GetDynamicSpecSourceTags().AddTag(GenshinAbility->InputTag);
		}
	}
	return GiveAbility(Spec);
}

FGameplayAbilitySpecHandle UTwoAbilitySystemComponentBase::K2_GiveAbilityWithTag(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level,
                                                                                 int32 InputID)
{
	// 构建并验证 Ability Spec
	FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(AbilityClass, Level, InputID);

	// 验证类
	if (!IsValid(AbilitySpec.Ability))
	{
		ABILITY_LOG(Error, TEXT("K2_GiveAbility() called with an invalid Ability Class."));

		return FGameplayAbilitySpecHandle();
	}

	// 授予 ability 并返回 handle。这将运行验证和权限检查
	return GiveAbilityWithTag(AbilitySpec);
}

void UTwoAbilitySystemComponentBase::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) //循环查找并激活包含有对应InputTag的Ability
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec); //通知Ability内的输入按下
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle); //并且如果能力未被激活,就激活一次
			}
		}
	}
}

void UTwoAbilitySystemComponentBase::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) //循环查找并激活包含有对应InputTag的Ability
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec); //通知Ability内的输入按下
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle); //并且如果能力未被激活,就每帧激活一次
			}
		}
	}
}

void UTwoAbilitySystemComponentBase::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) //循环查找并激活包含有对应InputTag的Ability
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec); //通知Ability内的输入释放
		}
	}
}


void UTwoAbilitySystemComponentBase::BeginPlay()
{
	Super::BeginPlay();
}


void UTwoAbilitySystemComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
