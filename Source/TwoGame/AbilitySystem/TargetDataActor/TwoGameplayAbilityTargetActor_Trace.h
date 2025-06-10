// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TwoGameplayAbilityTargetActor_Trace.generated.h"

UCLASS()
class TWOGAME_API ATwoGameplayAbilityTargetActor_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ATwoGameplayAbilityTargetActor_Trace();

	// 基础发散角度
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float BaseSpread;

	// 瞄准发散角度修正
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float AimingSpreadMod;

	// 持续射击的发散角度增量
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadIncrement;

	// 持续射击的最大发散角度
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadMax;

protected:
	// 当前的发散角度
	float CurrentTargetingSpread;

public:
	// 使用瞄准发散角度修正
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	bool bUseAimingSpreadMod;

	// 瞄准标签
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingTag;

	// 瞄准标签移除标签
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingRemovalTag;

	// 最大射程
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	float MaxRange;

	// 碰撞配置
	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = "Trace")
	FCollisionProfileName TraceProfile;

	// 轨迹是否影响瞄准俯仰
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceAffectsAimPitch;

	// 每个跟踪返回的最大命中结果数。0 仅返回跟踪终点。
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	int32 MaxHitResultsPerTrace;

	/**一次要执行的跟踪数。像 Rifles 这样的单发子弹武器只会进行一次跟踪。
	多子弹武器（如霰弹枪）可以执行多次跟踪。不适用于 持续点击。
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	int32 NumberOfTraces;

	// 是否忽略阻塞命中
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bIgnoreBlockingHits;

	// 是否从玩家视点开始跟踪
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceFromPlayerViewPoint;

	// HitResults 将一直存在，直到确认/取消或新的 HitResult 取代它
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bUsePersistentHitResults;

	// 重置发散角度
	UFUNCTION(BlueprintCallable)
	virtual void ResetSpread();

	// 获取当前发散角度
	virtual float GetCurrentSpread() const;

	UFUNCTION(BlueprintCallable)
	void SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation)
	{
		StartLocation = InStartLocation;
	}

	UFUNCTION(BlueprintCallable)
	virtual void SetShouldProduceTargetDataOnServer(bool bInShouldProduceTargetDataOnServer)
	{
		ShouldProduceTargetDataOnServer = bInShouldProduceTargetDataOnServer;
	}

	UFUNCTION(BlueprintCallable)
	void SetDestroyOnConfirmation(bool bInDestroyOnConfirmation = false)
	{
		bDestroyOnConfirmation = bInDestroyOnConfirmation;
	}

	// 开始运行获取TargetData
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	// 确认目标And继续
	virtual void ConfirmTargetingAndContinue() override;

	// 取消目标
	virtual void CancelTargeting() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	// 照常进行跟踪，但会手动过滤所有传入的演员
	virtual void LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle,
	                                 const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);

	// 使用玩家控制器瞄准
	virtual void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd,
	                                     bool bIgnorePitch = false);

	// 剪切相机射线以适应技能范围
	virtual bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange,
	                                         FVector& ClippedPosition);

	/**
	UFUNCTION(BlueprintCallable, Category = "Ability|Targets")
	virtual AGAbilityTargetActor_Trace* ConfigSetting();
	这个将会在子类里面实现
	*/

	// 停止目标
	virtual void StopTargeting();

protected:
	// 追踪的末尾用于调试绘图
	FVector CurrentTraceEnd;

	// 十字线Actor
	TArray<TWeakObjectPtr<AGameplayAbilityWorldReticle>> ReticleActors;

	// 持久的命中结果
	TArray<FHitResult> PersistentHitResults;

	// 创建目标数据
	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;

	//重要函数 执行跟踪
	virtual TArray<FHitResult> PerformTrace(AActor* InSourceActor);

	//执行跟踪
	virtual void DoTrace(TArray<FHitResult>& HitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle,
	                     const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params) PURE_VIRTUAL(
		AGSGATA_Trace, return;);

	// 显示调试跟踪
	virtual void ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = 2.0f) PURE_VIRTUAL(
		AGSGATA_Trace, return;);

	//  生成准星 Actor
	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);

	//  销毁标线 Actor
	virtual void DestroyReticleActors();
};
