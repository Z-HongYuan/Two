// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoCharacterBase.h"

ATwoCharacterBase::ATwoCharacterBase(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UTwoCMC>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bPropagateCurvesToFollowers = true; // 曲线同步到子Mesh
	GetMesh()->SetCollisionProfileName("NoCollision"); //无碰撞
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision); //无碰撞
	GetMesh()->CastShadow = false; //不投射阴影
	GetMesh()->SetVisibleFlag(false); //不可见
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	//总是刷新动画和骨骼

	CoverMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CoverMesh")); //创建CoverMesh
	CoverMesh->SetupAttachment(GetMesh()); //附加到原本的Mesh上
	CoverMesh->SetRenderCustomDepth(true); //启用自定义深度渲染

	WeaponComponent = CreateDefaultSubobject<UTwoWeaponComponent>(TEXT("WeaponComponent"));
}

void ATwoCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

//父类中不做任何事情,将会在子类中实现
void ATwoCharacterBase::InitAbilitySystemComponent()
{
}

void ATwoCharacterBase::InitAttributeSet() const
{
	if (GetAbilitySystemComponent() and StartupAttribute)
	{
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
			*GetAbilitySystemComponent()->MakeOutgoingSpec(StartupAttribute, -1, GetAbilitySystemComponent()->MakeEffectContext()).Data.Get(),
			GetAbilitySystemComponent()
		);
	}
}

void ATwoCharacterBase::InitStartAbilitiesAndEffects()
{
	//StartUp GE
	for (TSubclassOf<UGameplayEffect>& Effect : StartupEffects)
	{
		if (!Effect) continue;
		FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(Effect, -1, ContextHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	}

	//服务端Give
	if (HasAuthority())
	{
		for (TSubclassOf<UGameplayAbility>& Ability : StartupAbilities)
		{
			if (!Ability) continue;
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, -1, -1, this);
			GetAbilitySystemComponent()->GiveAbilityWithTag(AbilitySpec);
		}
	}

	//服务端Give
	if (HasAuthority())
	{
		for (TSubclassOf<UGameplayAbility>& Ability : StartupDoOnceAbilities)
		{
			if (!Ability) continue;
			FGameplayAbilitySpec AbilitySpec(Ability, -1, -1, this);
			GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(AbilitySpec);
		}
	}
}

void ATwoCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwoCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
