// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "TwoGame/AbilitySystem/TwoAbilitySystemComponentBase.h"
#include "TwoPlayerControllerBase.generated.h"

struct FGameplayTag;
class UInputMappingContext;
class UTwoInputDataAsset;

/**
 * 带有基础输入的玩家控制器
 * 实现ASC的输入触发
 */
UCLASS()
class TWOGAME_API ATwoPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	ATwoPlayerControllerBase();

	//视角缩放
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float LeftRightRate = 0.75;

	//视角缩放
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float UpDownRate = 0.75;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

protected:
	//优先级为 10 的输入上下文
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	//Ability与Tag对应的数据资产
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UTwoInputDataAsset> InputDataAsset;

	//默认需要的Move/Look回调函数
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);

	//用于在ASC内触发输入的函数
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	//观察者模式的TWeakObjectPtr的ASC,用于Get
	TWeakObjectPtr<UTwoAbilitySystemComponentBase> GenshinAbilitySystemComponent;
	UTwoAbilitySystemComponentBase* GetASC();
};
