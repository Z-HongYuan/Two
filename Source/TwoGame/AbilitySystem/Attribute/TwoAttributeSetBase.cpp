// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoAttributeSetBase.h"
#include "Net/UnrealNetwork.h"

UTwoAttributeSetBase::UTwoAttributeSetBase()
{
}

void UTwoAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*`REPNOTIFY_Always` 告诉 `OnRep` 函数在本地值已经等于从服务器复制的值（由于预测）时触发。默认情况下，如果本地值与从服务器复制的值相同，则不会触发 `OnRep` 函数。*/
	DOREPLIFETIME_CONDITION_NOTIFY(UTwoAttributeSetBase, Health, COND_None, REPNOTIFY_Always); //确保属性终身复制
}

void UTwoAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTwoAttributeSetBase, Health, OldHealth);
}
