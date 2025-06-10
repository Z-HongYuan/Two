// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TwoAttributeSetBase.generated.h"

/**
 * 此宏定义一组用于访问和初始化属性的辅助函数。
 *
 * 以下宏示例：
 * ATTRIBUTE_ACCESSORS（ULyraHealthSet， 生命值）
 * 将创建以下函数：
 * static FGameplayAttribute GetHealthAttribute（）;
 * float GetHealth（） const;
 * void SetHealth（float NewVal）;
 * void InitHealth（float NewVal）;
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
// ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)

/*
 * 基类Attribute
 * 带有基础的血量值
 */
UCLASS()
class TWOGAME_API UTwoAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTwoAttributeSetBase();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//生命值
	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTwoAttributeSetBase, Health)
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	//最大生命值
	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTwoAttributeSetBase, MaxHealth)
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	//移动速度
	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MaxMoveSpeed)
	FGameplayAttributeData MaxMoveSpeed;
	ATTRIBUTE_ACCESSORS(UTwoAttributeSetBase, MaxMoveSpeed)
	UFUNCTION()
	virtual void OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
