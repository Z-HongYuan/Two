// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "../TwoCharacterBase.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "TwoGame/Component/TwoSDFComponent.h"
#include "TwoPlayerBase.generated.h"

/*
 * 用于玩家操控的基础角色
 * 继承基类的ASC和CoverMesh
 * 添加SDFComponent
 */
UCLASS()
class TWOGAME_API ATwoPlayerBase : public ATwoCharacterBase
{
	GENERATED_BODY()

public:
	ATwoPlayerBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="Components")
	TObjectPtr<UTwoSDFComponent> SDFComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="Components")
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;

	virtual void PossessedBy(AController* NewController) override;

	//仅限拥有的客户端
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void SetupCamera_RunOnClient();
	void SetupCamera_RunOnClient_Implementation();

	virtual void OnRep_PlayerState() override;

	virtual void InitAbilitySystemComponent() override; //初始化ASC入口
};
