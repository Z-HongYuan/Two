// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TwoGame/AbilitySystem/TwoAbilitySystemComponentBase.h"
#include "TwoGame/AbilitySystem/Attribute/TwoAttributeSetBase.h"
#include "TwoGame/Weapon/TwoWeaponComponent.h"
#include "TwoGame/Component/TwoCMC.h"
#include "TwoCharacterBase.generated.h"

/*
 * ATwoCharacterBase
 * 基类Character,带有CoverMesh和ASC(需要子类创建)
 */

UCLASS(Abstract)
class TWOGAME_API ATwoCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATwoCharacterBase(const FObjectInitializer& ObjectInitializer);

	/*
	 * ASC和Attribute
	 */
	virtual UTwoAbilitySystemComponentBase* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UTwoAttributeSetBase* GetAttributeSet() const { return AttributeSet; }

	/*
	 * 获取CoverMesh
	 */
	UFUNCTION(BlueprintPure, Category="TwoFunction")
	USkeletalMeshComponent* GetCoverMesh() { return CoverMesh; };

	//武器组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UTwoWeaponComponent> WeaponComponent;

protected:
	virtual void BeginPlay() override;

	/*
	 * ASC和Attribute
	 */
	UPROPERTY()
	TObjectPtr<UTwoAbilitySystemComponentBase> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UTwoAttributeSetBase> AttributeSet;

	//复制动画的表现层Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="Character")
	TObjectPtr<USkeletalMeshComponent> CoverMesh;

	//初始属性集
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> StartupAttribute;

	//初始GE效果
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	//初始能力
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	//初始能力并且执行一次
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> StartupDoOnceAbilities;

	//初始化AbilitySystemComponent,不做任何事,将在子类中实现
	virtual void InitAbilitySystemComponent();
	void InitAttributeSet() const; //必须调用,全部Give的Level=-1,使用GE初始化AttributeSet
	void InitStartAbilitiesAndEffects(); //必须调用,全部Give的Level=-1
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
