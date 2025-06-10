// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoGameplayAbilityTargetActor_Trace.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "Abilities/GameplayAbility.h"


ATwoGameplayAbilityTargetActor_Trace::ATwoGameplayAbilityTargetActor_Trace()
{
	bDestroyOnConfirmation = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
	MaxHitResultsPerTrace = 1;
	NumberOfTraces = 1;
	bIgnoreBlockingHits = false;
	bTraceAffectsAimPitch = true;
	bTraceFromPlayerViewPoint = false;
	MaxRange = 999999.0f;
	bUseAimingSpreadMod = false;
	BaseSpread = 0.0f;
	AimingSpreadMod = 0.0f;
	TargetingSpreadIncrement = 0.0f;
	TargetingSpreadMax = 0.0f;
	CurrentTargetingSpread = 0.0f;
	bUsePersistentHitResults = false;
}

void ATwoGameplayAbilityTargetActor_Trace::ResetSpread()
{
	bUseAimingSpreadMod = false;
	BaseSpread = 0.0f;
	AimingSpreadMod = 0.0f;
	TargetingSpreadIncrement = 0.0f;
	TargetingSpreadMax = 0.0f;
	CurrentTargetingSpread = 0.0f;
}

float ATwoGameplayAbilityTargetActor_Trace::GetCurrentSpread() const
{
	float FinalSpread = BaseSpread + CurrentTargetingSpread;
	if (bUseAimingSpreadMod && AimingTag.IsValid() && AimingRemovalTag.IsValid())
	{
		UAbilitySystemComponent* ASC = OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent.Get();
		if (ASC && (ASC->GetTagCount(AimingTag) > ASC->GetTagCount(AimingRemovalTag)))
		{
			//Aim模式下*倍率
			FinalSpread *= AimingSpreadMod;
		}
	}

	return FinalSpread;
}

void ATwoGameplayAbilityTargetActor_Trace::StartTargeting(UGameplayAbility* Ability)
{
	// Super::StartTargeting(Ability);
	// 不要Super，因为我们可以有多个 Reticle

	SetActorTickEnabled(true);

	OwningAbility = Ability;
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	// 这是清空和重新填充 ReticleActor 的一种懒惰方式。
	// 我们可以想出一个可以重复使用它们的解决方案。
	DestroyReticleActors();

	if (ReticleClass)
	{
		for (int32 i = 0; i < MaxHitResultsPerTrace * NumberOfTraces; i++)
		{
			SpawnReticleActor(GetActorLocation(), GetActorRotation());
		}
	}

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void ATwoGameplayAbilityTargetActor_Trace::ConfirmTargetingAndContinue()
{
	// Super::ConfirmTargetingAndContinue();不要Super自己实现
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		TArray<FHitResult> HitResults = PerformTrace(SourceActor);
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResults);
		TargetDataReadyDelegate.Broadcast(Handle);

#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForDuration, 2.0f);
		}
#endif
	}

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void ATwoGameplayAbilityTargetActor_Trace::CancelTargeting()
{
	const FGameplayAbilityActorInfo* ActorInfo = (OwningAbility ? OwningAbility->GetCurrentActorInfo() : nullptr);
	UAbilitySystemComponent* ASC = (ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr);
	if (ASC)
	{
		ASC->AbilityReplicatedEventDelegate(
			EAbilityGenericReplicatedEvent::GenericCancel,
			OwningAbility->GetCurrentAbilitySpecHandle(),
			OwningAbility->GetCurrentActivationInfo().GetActivationPredictionKey()).Remove(GenericCancelHandle
		);
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("AGameplayAbilityTargetActor::CancelTargeting called with null ASC! Actor %s"), *GetName());
	}

	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle());

	SetActorTickEnabled(false);

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void ATwoGameplayAbilityTargetActor_Trace::BeginPlay()
{
	Super::BeginPlay();
	/** 从 Tick disabled 开始。我们将在 StartTargeting() 中启用它，并在 StopTargeting() 中再次禁用它。
	对于即时确认，Tick永远不会发生，因为我们 StartTargeting()、ConfirmTargeting() 和立即 StopTargeting()。
	*/
	SetActorTickEnabled(false);
}

void ATwoGameplayAbilityTargetActor_Trace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<FHitResult> HitResults;
	if (bDebug || bUsePersistentHitResults)
	{
		// 如果我们显示调试或使用持久命中结果，则只需在 Tick 上进行跟踪，否则我们只使用确认跟踪
		HitResults = PerformTrace(SourceActor);
	}

#if ENABLE_DRAW_DEBUG
	if (SourceActor && bDebug)
	{
		ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForOneFrame);
	}
#endif
}

void ATwoGameplayAbilityTargetActor_Trace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyReticleActors();
	Super::EndPlay(EndPlayReason);
}

void ATwoGameplayAbilityTargetActor_Trace::LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World,
                                                               const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start,
                                                               const FVector& End, FName ProfileName,
                                                               const FCollisionQueryParams Params)
{
	check(World);

	TArray<FHitResult> HitResults;
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);

	TArray<FHitResult> FilteredHitResults;

	// Start param 可以是玩家 ViewPoint。我们希望 HitResult 始终显示 StartLocation。
	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();

	for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
	{
		FHitResult& Hit = HitResults[HitIdx];

		if (!IsValid(Hit.GetActor()) || FilterHandle.FilterPassesForActor(Hit.GetActor()))
		{
			Hit.TraceStart = TraceStart;
			Hit.TraceEnd = End;

			FilteredHitResults.Add(Hit);
		}
	}

	OutHitResults = FilteredHitResults;
}

void ATwoGameplayAbilityTargetActor_Trace::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params,
                                                                   const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch)
{
	if (!OwningAbility) // 仅限 Server 和 Launch Client
	{
		return;
	}

	// AI Controller 的默认值
	FVector ViewStart = TraceStart;
	FRotator ViewRot = StartLocation.GetTargetingTransform().GetRotation().Rotator();

	if (PrimaryPC)
	{
		PrimaryPC->GetPlayerViewPoint(ViewStart, ViewRot);
	}

	const FVector ViewDir = ViewRot.Vector();
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);

	ClipCameraRayToAbilityRange(ViewStart, ViewDir, TraceStart, MaxRange, ViewEnd);

	// 使用首次碰撞
	TArray<FHitResult> HitResults;
	LineTraceWithFilter(HitResults, InSourceActor->GetWorld(), Filter, ViewStart, ViewEnd, TraceProfile.Name, Params);

	CurrentTargetingSpread = FMath::Min(TargetingSpreadMax, CurrentTargetingSpread + TargetingSpreadIncrement);

	const bool bUseTraceResult = HitResults.Num() > 0 && (FVector::DistSquared(TraceStart, HitResults[0].Location) <= (MaxRange * MaxRange));

	const FVector AdjustedEnd = (bUseTraceResult) ? HitResults[0].Location : ViewEnd;

	FVector AdjustedAimDir = (AdjustedEnd - TraceStart).GetSafeNormal();
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}

	if (!bTraceAffectsAimPitch && bUseTraceResult)
	{
		FVector OriginalAimDir = (ViewEnd - TraceStart).GetSafeNormal();

		if (!OriginalAimDir.IsZero())
		{
			// 转换为角度并使用原始螺距
			const FRotator OriginalAimRot = OriginalAimDir.Rotation();

			FRotator AdjustedAimRot = AdjustedAimDir.Rotation();
			AdjustedAimRot.Pitch = OriginalAimRot.Pitch;

			AdjustedAimDir = AdjustedAimRot.Vector();
		}
	}

	const float CurrentSpread = GetCurrentSpread();

	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const FVector ShootDir = WeaponRandomStream.VRandCone(AdjustedAimDir, ConeHalfAngle, ConeHalfAngle);

	OutTraceEnd = TraceStart + (ShootDir * MaxRange);
}

bool ATwoGameplayAbilityTargetActor_Trace::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter,
                                                                       float AbilityRange, FVector& ClippedPosition)
{
	FVector CameraToCenter = AbilityCenter - CameraLocation;
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);
	if (DotToCenter >= 0) //如果失败，我们将指向远离中心的位置，但我们可能在球体内并能够找到一个好的出口点。
	{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter);
		float RadiusSquared = (AbilityRange * AbilityRange);
		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared);
			float DistanceAlongRay = DotToCenter + DistanceFromCamera; //减去而不是加法将获得另一个交点
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection); //剪切到范围球体的凸轮瞄准点	
			return true;
		}
	}
	return false;
}

void ATwoGameplayAbilityTargetActor_Trace::StopTargeting()
{
	SetActorTickEnabled(false);

	DestroyReticleActors();

	// 清除所有的回调
	TargetDataReadyDelegate.Clear();
	CanceledDelegate.Clear();

	if (GenericDelegateBoundASC)
	{
		GenericDelegateBoundASC->GenericLocalConfirmCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::ConfirmTargeting);
		GenericDelegateBoundASC->GenericLocalCancelCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::CancelTargeting);
		GenericDelegateBoundASC = nullptr;
	}
}

FGameplayAbilityTargetDataHandle ATwoGameplayAbilityTargetActor_Trace::MakeTargetData(const TArray<FHitResult>& HitResults) const
{
	FGameplayAbilityTargetDataHandle ReturnDataHandle;

	for (int32 i = 0; i < HitResults.Num(); i++)
	{
		/** 注意： 这些由 FGameplayAbilityTargetDataHandle 清理（通过内部 TSharedPtr） */
		FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
		ReturnData->HitResult = HitResults[i];
		ReturnDataHandle.Add(ReturnData);
	}

	return ReturnDataHandle;
}

TArray<FHitResult> ATwoGameplayAbilityTargetActor_Trace::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(InSourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGSGATA_LineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	Params.bIgnoreBlocks = bIgnoreBlockingHits;

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;

	if (PrimaryPC)
	{
		FVector ViewStart;
		FRotator ViewRot;
		PrimaryPC->GetPlayerViewPoint(ViewStart, ViewRot);

		TraceStart = bTraceFromPlayerViewPoint ? ViewStart : TraceStart;
	}

	if (bUsePersistentHitResults)
	{
		// Clear any blocking hit results, invalid Actors, or actors out of range
		//TODO Check for visibility if we add AIPerceptionComponent in the future
		for (int32 i = PersistentHitResults.Num() - 1; i >= 0; i--)
		{
			FHitResult& HitResult = PersistentHitResults[i];

			if (HitResult.bBlockingHit || !IsValid(HitResult.GetActor()) || FVector::DistSquared(TraceStart, HitResult.GetActor()->GetActorLocation())
				> (MaxRange * MaxRange))
			{
				PersistentHitResults.RemoveAt(i);
			}
		}
	}

	TArray<FHitResult> ReturnHitResults;

	for (int32 TraceIndex = 0; TraceIndex < NumberOfTraces; TraceIndex++)
	{
		AimWithPlayerController(InSourceActor, Params, TraceStart, TraceEnd); //Effective on server and launching client only

		// ------------------------------------------------------

		SetActorLocationAndRotation(TraceEnd, SourceActor->GetActorRotation());

		CurrentTraceEnd = TraceEnd;

		TArray<FHitResult> TraceHitResults;
		DoTrace(TraceHitResults, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);

		for (int32 j = TraceHitResults.Num() - 1; j >= 0; j--)
		{
			if (MaxHitResultsPerTrace >= 0 && j + 1 > MaxHitResultsPerTrace)
			{
				// Trim to MaxHitResultsPerTrace
				TraceHitResults.RemoveAt(j);
				continue;
			}

			FHitResult& HitResult = TraceHitResults[j];

			// Reminder: if bUsePersistentHitResults, Number of Traces = 1
			if (bUsePersistentHitResults)
			{
				// This is looping backwards so that further objects from player are added first to the queue.
				// This results in closer actors taking precedence as the further actors will get bumped out of the TArray.
				if (IsValid(HitResult.GetActor()) && (!HitResult.bBlockingHit || PersistentHitResults.Num() < 1))
				{
					bool bActorAlreadyInPersistentHits = false;

					// Make sure PersistentHitResults doesn't have this hit actor already
					for (int32 k = 0; k < PersistentHitResults.Num(); k++)
					{
						FHitResult& PersistentHitResult = PersistentHitResults[k];

						if (PersistentHitResult.GetActor() == HitResult.GetActor())
						{
							bActorAlreadyInPersistentHits = true;
							break;
						}
					}

					if (bActorAlreadyInPersistentHits)
					{
						continue;
					}

					if (PersistentHitResults.Num() >= MaxHitResultsPerTrace)
					{
						// Treat PersistentHitResults like a queue, remove first element
						PersistentHitResults.RemoveAt(0);
					}

					PersistentHitResults.Add(HitResult);
				}
			}
			else
			{
				// ReticleActors for PersistentHitResults are handled later
				int32 ReticleIndex = TraceIndex * MaxHitResultsPerTrace + j;
				if (ReticleIndex < ReticleActors.Num())
				{
					if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[ReticleIndex].Get())
					{
						const bool bHitActor = HitResult.GetActor() != nullptr;

						if (bHitActor && !HitResult.bBlockingHit)
						{
							LocalReticleActor->SetActorHiddenInGame(false);

							const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor)
								                                ? HitResult.GetActor()->GetActorLocation()
								                                : HitResult.Location;

							LocalReticleActor->SetActorLocation(ReticleLocation);
							LocalReticleActor->SetIsTargetAnActor(bHitActor);
						}
						else
						{
							LocalReticleActor->SetActorHiddenInGame(true);
						}
					}
				}
			}
		} // for TraceHitResults

		if (!bUsePersistentHitResults)
		{
			if (TraceHitResults.Num() < ReticleActors.Num())
			{
				// We have less hit results than ReticleActors, hide the extra ones
				for (int32 j = TraceHitResults.Num(); j < ReticleActors.Num(); j++)
				{
					if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[j].Get())
					{
						LocalReticleActor->SetIsTargetAnActor(false);
						LocalReticleActor->SetActorHiddenInGame(true);
					}
				}
			}
		}

		if (TraceHitResults.Num() < 1)
		{
			// If there were no hits, add a default HitResult at the end of the trace
			FHitResult HitResult;
			// Start param could be player ViewPoint. We want HitResult to always display the StartLocation.
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();
			HitResult.TraceEnd = TraceEnd;
			HitResult.Location = TraceEnd;
			HitResult.ImpactPoint = TraceEnd;
			TraceHitResults.Add(HitResult);

			if (bUsePersistentHitResults && PersistentHitResults.Num() < 1)
			{
				PersistentHitResults.Add(HitResult);
			}
		}

		ReturnHitResults.Append(TraceHitResults);
	} // for NumberOfTraces

	// Reminder: if bUsePersistentHitResults, Number of Traces = 1
	if (bUsePersistentHitResults && MaxHitResultsPerTrace > 0)
	{
		// Handle ReticleActors
		for (int32 PersistentHitResultIndex = 0; PersistentHitResultIndex < PersistentHitResults.Num(); PersistentHitResultIndex++)
		{
			FHitResult& HitResult = PersistentHitResults[PersistentHitResultIndex];

			// Update TraceStart because old persistent HitResults will have their original TraceStart and the player could have moved since then
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();

			if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
			{
				const bool bHitActor = HitResult.GetActor() != nullptr;

				if (bHitActor && !HitResult.bBlockingHit)
				{
					LocalReticleActor->SetActorHiddenInGame(false);

					const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor)
						                                ? HitResult.GetActor()->GetActorLocation()
						                                : HitResult.Location;

					LocalReticleActor->SetActorLocation(ReticleLocation);
					LocalReticleActor->SetIsTargetAnActor(bHitActor);
				}
				else
				{
					LocalReticleActor->SetActorHiddenInGame(true);
				}
			}
		}

		if (PersistentHitResults.Num() < ReticleActors.Num())
		{
			// We have less hit results than ReticleActors, hide the extra ones
			for (int32 PersistentHitResultIndex = PersistentHitResults.Num(); PersistentHitResultIndex < ReticleActors.Num(); PersistentHitResultIndex
			     ++)
			{
				if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
				{
					LocalReticleActor->SetIsTargetAnActor(false);
					LocalReticleActor->SetActorHiddenInGame(true);
				}
			}
		}

		return PersistentHitResults;
	}

	return ReturnHitResults;
}

AGameplayAbilityWorldReticle* ATwoGameplayAbilityTargetActor_Trace::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, Location, Rotation);
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);
			SpawnedReticleActor->SetActorHiddenInGame(true);
			ReticleActors.Add(SpawnedReticleActor);

			// This is to catch cases of playing on a listen server where we are using a replicated reticle actor.
			// (In a client controlled player, this would only run on the client and therefor never replicate. If it runs
			// on a listen server, the reticle actor may replicate. We want consistancy between client/listen server players.
			// Just saying 'make the reticle actor non replicated' isnt a good answer, since we want to mix and match reticle
			// actors and there may be other targeting types that want to replicate the same reticle actor class).
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}

			return SpawnedReticleActor;
		}
	}

	return nullptr;
}

void ATwoGameplayAbilityTargetActor_Trace::DestroyReticleActors()
{
	for (int32 i = ReticleActors.Num() - 1; i >= 0; i--)
	{
		if (ReticleActors[i].IsValid())
		{
			ReticleActors[i].Get()->Destroy();
		}
	}

	ReticleActors.Empty();
}
