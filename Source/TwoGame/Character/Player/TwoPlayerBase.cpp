// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoPlayerBase.h"

#include "TwoGame/Gameplay/PlayerState/TwoPlayerStateBase.h"


ATwoPlayerBase::ATwoPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true; //启动复制

	SDFComponent = CreateDefaultSubobject<UTwoSDFComponent>(TEXT("SDFComponent")); //创建SDF组件
	SDFComponent->SetupAttachment(GetCoverMesh());

	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCamera"));
	GameplayCamera->SetupAttachment(GetCoverMesh());
}

void ATwoPlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATwoPlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystemComponent();
	SetupCamera_RunOnClient();
}

void ATwoPlayerBase::SetupCamera_RunOnClient_Implementation()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (GameplayCamera)
		{
			GameplayCamera->ActivateCameraForPlayerController(PlayerController);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AGenshinPlayer::SetupCamera_Implementation() GameplayCamera is nullptr"));
			PlayerController->SetViewTargetWithBlend(this);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenshinPlayer::SetupCamera_Implementation() PlayerController is nullptr"));
	}
}

void ATwoPlayerBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystemComponent();
	SetupCamera_RunOnClient();
}

void ATwoPlayerBase::InitAbilitySystemComponent()
{
	if (ATwoPlayerStateBase* PS = GetPlayerStateChecked<ATwoPlayerStateBase>())
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AttributeSet = PS->GetAttributeSet();
		if (AbilitySystemComponent) AbilitySystemComponent->InitAbilityActorInfo(PS, this); // 设置ASC信息
		AbilitySystemComponent->SetIsReplicated(true); //启动复制
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); //设置ASC复制模式 多人控制的Mode

		//log
		UE_LOG(LogTemp, Log, TEXT("AGenshinPlayer::InitAbilitySystemComponent() is success"));
	}
	else
	{
		//log
		UE_LOG(LogTemp, Warning, TEXT("AGenshinPlayer::InitAbilitySystemComponent() is Not Begin"));
	}

	InitAttributeSet();
	InitStartAbilitiesAndEffects();
}
