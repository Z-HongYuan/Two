// Copyright (c) 2025 鸿源z. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TwoSDFComponent.generated.h"

/*
 * 用于特定Mesh上使用的SDF组件
 * 需要材质支持SDF
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOGAME_API UTwoSDFComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UTwoSDFComponent();

	//脸部材质ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SDF")
	int FaceID;

	//需要变化的材质属性名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SDF")
	FName FaceForwardValueName;

	//需要变化的材质属性名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SDF")
	FName FaceRightValueName;

	//骨骼插槽名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SDF")
	FName FaceForwardName;

	//骨骼插槽名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SDF")
	FName FaceRightName;

	//骨骼插槽名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SDF")
	FName HeadName;

	//刷新面部材质实例,在更换角色的时候非常有用
	UFUNCTION(BlueprintCallable, Category = "SDF")
	void RefreshFaceMaterialInstance();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	TWeakObjectPtr<USkeletalMeshComponent> ParentMeshComponent;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> FaceMaterialInstance;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
