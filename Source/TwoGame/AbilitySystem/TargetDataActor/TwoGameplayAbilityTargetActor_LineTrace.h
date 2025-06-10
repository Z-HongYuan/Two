﻿// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TwoGameplayAbilityTargetActor_Trace.h"
#include "TwoGameplayAbilityTargetActor_LineTrace.generated.h"

UCLASS()
class TWOGAME_API ATwoGameplayAbilityTargetActor_LineTrace : public ATwoGameplayAbilityTargetActor_Trace
{
	GENERATED_BODY()

public:
	ATwoGameplayAbilityTargetActor_LineTrace();
	/**
		配置目标角色（TargetActor）以供使用。此目标角色可在多个技能（abilities）中使用，但无法保证
		其处于何种状态。您需要确保一次仅有一个技能正在使用此目标角色。
		@param InStartLocation 追踪起始位置。
		@param InAimingTag 可选参数。用于瞄准的预测游戏标签（Predicted GameplayTag）。仅在瞄准期间修改散布范围时使用。如果使用此参数，
		则必须同时设置 InAimingRemovalTag。
		@param InAimingRemovalTag 可选参数。用于移除瞄准状态的预测游戏标签。仅在瞄准期间修改散布范围时使用。如果使用此参数，
		则必须同时设置 InAimingTag。
		@param InTraceProfile 追踪时使用的碰撞配置文件。
		@param InFilter 命中的角色必须通过此过滤器，才能在目标数据（TargetData）中返回。
		@param InReticleClass 出现在已获取目标上方的准星类。当目标被获取或丢失时，准星将被生成或销毁。
		@param InReticleParams 世界准星的参数。这些参数的使用取决于准星的实现。
		@param bInIgnoreBlockingHits 在追踪过程中忽略阻挡碰撞的命中。如果您想透过墙壁进行目标选择，此参数很有用。
		@param bInShouldProduceTargetDataOnServer 如果设置为 true，此目标角色除了在客户端生成目标数据外，还将在服务器端生成目标数据，
		客户端只需向服务器发送一个通用的 “确认” 事件。如果为 false，客户端将把目标数据发送到服务器。这由 WaitTargetDataUsingActor 技能任务处理。
		@param bInUsePersistentHitResults 在目标选择期间，命中结果是否应持续存在？命中结果会在确认 / 取消操作时或被新的命中结果替代时被清除。
		@param bInDebug 若为 true，此目标角色将显示追踪和命中结果的调试线。
		@param bInTraceAffectsAimPitch 追踪是否会影响瞄准的俯仰角？
		@param bInTraceFromPlayerViewPoint 我们是否应从玩家视角而不是起始位置进行追踪？
		目标数据的命中结果仍将以起始位置作为追踪起点。这对于第一人称射击游戏（FPS）很有用，因为我们希望从玩家视角进行追踪，但从武器枪口绘制子弹轨迹。
		待办事项：AI 控制器应回退到枪口位置。目前尚未实现。
		@param bInUseAImingSpreadMode 我们是否应根据是否正在瞄准来修改散布范围？如果为 true，必须设置 InAimingTag 和 InAimingRemovalTag。
		@param InMaxRange 此追踪的最大范围。
		@param InBaseSpread 基础目标散布范围（以度为单位）。
		@param InAimingSpreadMod 可选参数。瞄准状态下对散布范围的乘法修正系数。
		@param InTargetingSpreadIncrement 持续目标选择时散布范围的增量（以度为单位）。
		@param InTargetingSpreadMax 持续目标选择时散布范围的最大增量（以度为单位）。
		@param InMaxHitResultsPerTrace 一次追踪可返回的最大命中结果数。小于 1 时仅返回追踪终点。
		@param InNumberOfTraces 要执行的追踪次数。旨在与基础散布范围配合使用，适用于霰弹枪等多弹丸武器。
		不建议与持久命中结果（PersistentHitResults）一起使用。如果使用持久命中结果，追踪次数将硬编码为 1。如果您需要此功能，需要在项目中自行添加支持。
	*/

	UFUNCTION(BlueprintCallable)
	void Configure(
		UPARAM(DisplayName = "Start Location")
		const FGameplayAbilityTargetingLocationInfo& InStartLocation,
		UPARAM(DisplayName = "Aiming Tag")
		FGameplayTag InAimingTag,
		UPARAM(DisplayName = "Aiming Removal Tag")
		FGameplayTag InAimingRemovalTag,
		UPARAM(DisplayName = "Trace Profile")
		FCollisionProfileName InTraceProfile,
		UPARAM(DisplayName = "Filter")
		FGameplayTargetDataFilterHandle InFilter,
		UPARAM(DisplayName = "Reticle Class")
		TSubclassOf<AGameplayAbilityWorldReticle> InReticleClass,
		UPARAM(DisplayName = "Reticle Params")
		FWorldReticleParameters InReticleParams,
		UPARAM(DisplayName = "Ignore Blocking Hits")
		bool bInIgnoreBlockingHits = false,
		UPARAM(DisplayName = "Should Produce Target Data on Server")
		bool bInShouldProduceTargetDataOnServer = false,
		UPARAM(DisplayName = "Use Persistent Hit Results")
		bool bInUsePersistentHitResults = false,
		UPARAM(DisplayName = "Debug")
		bool bInDebug = false,
		UPARAM(DisplayName = "Trace Affects Aim Pitch")
		bool bInTraceAffectsAimPitch = true,
		UPARAM(DisplayName = "Trace From Player ViewPoint")
		bool bInTraceFromPlayerViewPoint = false,
		UPARAM(DisplayName = "Use Aiming Spread Mod")
		bool bInUseAimingSpreadMod = false,
		UPARAM(DisplayName = "Max Range")
		float InMaxRange = 999999.0f,
		UPARAM(DisplayName = "Base Targeting Spread")
		float InBaseSpread = 0.0f,
		UPARAM(DisplayName = "Aiming Spread Mod")
		float InAimingSpreadMod = 0.0f,
		UPARAM(DisplayName = "Targeting Spread Increment")
		float InTargetingSpreadIncrement = 0.0f,
		UPARAM(DisplayName = "Targeting Spread Max")
		float InTargetingSpreadMax = 0.0f,
		UPARAM(DisplayName = "Max Hit Results Per Trace")
		int32 InMaxHitResultsPerTrace = 1,
		UPARAM(DisplayName = "Number of Traces")
		int32 InNumberOfTraces = 1
	);

protected:
	virtual void DoTrace(TArray<FHitResult>& HitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle,
	                     const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params) override
	{
		LineTraceWithFilter(HitResults, World, FilterHandle, Start, End, ProfileName, Params);
	}

	virtual void ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = 2.0f) override;

#if ENABLE_DRAW_DEBUG
	// Util for drawing result of multi line trace from KismetTraceUtils.h
	void DrawDebugLineTraceMulti(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit,
	                             const TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
#endif // ENABLE_DRAW_DEBUG
};
