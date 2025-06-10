// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UTwoAttributeSetBase::UTwoAttributeSetBase()
{
}

void UTwoAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*`REPNOTIFY_Always` 告诉 `OnRep` 函数在本地值已经等于从服务器复制的值（由于预测）时触发。默认情况下，如果本地值与从服务器复制的值相同，则不会触发 `OnRep` 函数。*/
	DOREPLIFETIME_CONDITION_NOTIFY(UTwoAttributeSetBase, Health, COND_None, REPNOTIFY_Always); //确保属性终身复制
	DOREPLIFETIME_CONDITION_NOTIFY(UTwoAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTwoAttributeSetBase, MaxMoveSpeed, COND_None, REPNOTIFY_Always);
}

void UTwoAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTwoAttributeSetBase, Health, OldHealth);
}

void UTwoAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTwoAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UTwoAttributeSetBase::OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTwoAttributeSetBase, MaxMoveSpeed, OldMoveSpeed);
}

void UTwoAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetMaxMoveSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 9999.0f); //限制移动速度
	}
}

void UTwoAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetMaxMoveSpeedAttribute())
	{
		SetMaxMoveSpeed(FMath::Clamp(GetMaxMoveSpeed(), 0.0f, 9999.0f));
	}
}
