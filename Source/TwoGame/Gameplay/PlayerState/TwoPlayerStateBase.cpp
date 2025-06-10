// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoPlayerStateBase.h"

ATwoPlayerStateBase::ATwoPlayerStateBase()
{
	bReplicates = true; //设置复制
	SetNetUpdateFrequency(200.f); //设置网络同步速度
	SetMinNetUpdateFrequency(100.f); //设置网络同步速度

	AbilitySystemComponent = CreateDefaultSubobject<UTwoAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true); //启动复制
	AttributeSet = CreateDefaultSubobject<UTwoAttributeSetBase>(TEXT("AttributeSet"));
}
