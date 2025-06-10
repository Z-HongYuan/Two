// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoFadingActor.h"
#include "TwoGame/Character/TwoCharacterBase.h"


ATwoFadingActor::ATwoFadingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));

	FadeMaterialStartValue = -1.0f;
	FadeMaterialEndValue = 1.0f;
	FadeStartDelayTime = FadeEndDelayTime = 1.0f;
	bIsBeginFade = false;
	FadeMaterialName = "FadingAlphaControl";
}

void ATwoFadingActor::BeginPlay()
{
	Super::BeginPlay();

	if (ATwoCharacterBase* CharacterBase = Cast<ATwoCharacterBase>(GetOwner()))
	{
		PoseableMesh->SetSkinnedAssetAndUpdate(CharacterBase->GetCoverMesh()->GetSkeletalMeshAsset());
		PoseableMesh->CopyPoseFromSkeletalComponent(CharacterBase->GetCoverMesh());

		//淡化前的延迟
		GetWorldTimerManager().SetTimer(FadeStartDelayTimerHandle, this, &ThisClass::FadeStartDelay, 1, false, FadeStartDelayTime);
	}
	else
	{
		//log
		UE_LOG(LogTemp, Warning, TEXT("AGenshinFadingActor::BeginPlay() Owner is not GenshinCharacter"));
		this->Destroy();
	}
}

void ATwoFadingActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (FadeStartDelayTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FadeStartDelayTimerHandle);
		FadeStartDelayTimerHandle.Invalidate();
	}
	if (FadeEndDelayTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FadeEndDelayTimerHandle);
		FadeEndDelayTimerHandle.Invalidate();
	}

	Super::EndPlay(EndPlayReason);
}

void ATwoFadingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBeginFade)
	{
		FadeMaterialStartValue = FMath::FInterpTo(FadeMaterialStartValue, FadeMaterialEndValue, DeltaTime, FadeSpeed);
		PoseableMesh->SetScalarParameterValueOnMaterials(FadeMaterialName, FadeMaterialStartValue);
	}

	//淡化结束的定时器回调,无论如何,只会执行一次
	if (FMath::IsNearlyEqual(FadeMaterialStartValue, FadeMaterialEndValue, 0.001f) and bIsBeginFade)
	{
		bIsBeginFade = false;
		GetWorldTimerManager().SetTimer(FadeEndDelayTimerHandle, this, &ThisClass::FadeEndDelay, 1, false, FadeEndDelayTime);
	}
}
