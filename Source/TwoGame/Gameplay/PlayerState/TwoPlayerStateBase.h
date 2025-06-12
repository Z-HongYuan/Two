// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TwoGame/AbilitySystem/TwoAbilitySystemComponentBase.h"
#include "TwoGame/AbilitySystem/Attribute/TwoAttributeSetBase.h"
#include "TwoPlayerStateBase.generated.h"

/**
 * 针对于网络复制的ASC组件
 */
UCLASS()
class TWOGAME_API ATwoPlayerStateBase : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATwoPlayerStateBase();
	
	/*
	 * ASC和Attribute
	 */
	virtual UTwoAbilitySystemComponentBase* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UTwoAttributeSetBase* GetAttributeSet() const { return AttributeSet; }

protected:
	/*
	 * ASC和Attribute
	 */
	UPROPERTY()
	TObjectPtr<UTwoAbilitySystemComponentBase> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UTwoAttributeSetBase> AttributeSet;
};
