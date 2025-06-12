// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TwoFadingActor.generated.h"

/*
 * 渐隐的Actor
 * 复制Owner的Mesh,并且通过材质参数控制材质的透明度实现淡入淡出
 */
UCLASS()
class TWOGAME_API ATwoFadingActor : public AActor
{
	GENERATED_BODY()

public:
	ATwoFadingActor();

	//材质淡出的开始值,默认为-1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float FadeMaterialStartValue;

	//材质淡出的结束值,默认为1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float FadeMaterialEndValue;

	//需要修改的材质参数值名称,默认为为"BodyAlphaControl"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	FName FadeMaterialName;

	//淡出开始前等待的延迟,默认为1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float FadeStartDelayTime;

	//淡出完成后结束延迟,最后销毁,默认为1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float FadeEndDelayTime;

	//淡化速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float FadeSpeed;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PoseableMesh")
	TObjectPtr<UPoseableMeshComponent> PoseableMesh;

	//淡出控制bool
	bool bIsBeginFade;

	//定时器句柄
	FTimerHandle FadeStartDelayTimerHandle;
	FTimerHandle FadeEndDelayTimerHandle;

	//延迟开始定时器回调,控制淡出bool
	UFUNCTION()
	void FadeStartDelay() { bIsBeginFade = true; }

	//延迟结束定时器回调,控制销毁自身
	UFUNCTION()
	void FadeEndDelay() { this->Destroy(); }

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;
};
