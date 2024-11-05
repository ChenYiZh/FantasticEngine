// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "XRCameraComponent.generated.h"

/**
 * 
 */
UCLASS(HideCategories=(Mobility, Rendering, LOD), Blueprintable, ClassGroup="FantasticEngine", meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UXRCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	friend class UVRPawnOperationsComponent;

public:
	virtual void BeginPlay() override;

public:
	/** 水平旋转旋转原点 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraOptions,
		meta=(DisplayPriority=-1, EditCondition="!bLockToHmd", EditConditionHides))
	bool bUseOriginTurn = true;

	/** 垂直旋转旋转原点 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraOptions,
		meta=(DisplayPriority=-1, EditCondition="!bLockToHmd && bUseActorTurn", EditConditionHides))
	bool bUseOriginLookUp = false;

	/** 禁止移动 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraOptions,
		meta=(DisplayPriority=-1, EditCondition="!bLockToHmd", EditConditionHides))
	bool bLockXYZ = false;

	/** 至今只移动Z轴 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraOptions,
		meta=(DisplayPriority=-1, EditCondition="!bLockToHmd && !bLockXYZ", EditConditionHides))
	bool bLockZ = false;

	/** 移动时，移动角色 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraOptions,
		meta=(DisplayPriority=-1, EditCondition="!bLockToHmd && !bLockXYZ", EditConditionHides))
	bool bUseOriginMove = true;

	/** Z轴移动是移动相机的相对高度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraOptions,
		meta=(DisplayPriority=-1, EditCondition="!bLockToHmd && !bLockXYZ && !bLockZ", EditConditionHides))
	bool bUseCameraHeight = true;

protected:
	virtual void HandleXRCamera() override;

protected:
	/** 更新缓存的数据 */
	virtual void UpdateTransformInfo();

private:
	UPROPERTY()
	bool bInitialized = false;

	/** 操作管理类 */
	TWeakObjectPtr<class UVRPawnOperationsComponent> OperationsComponent;

#pragma region 缓存数据

private:
	/** Actor原点 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FTransform ActorOrigin;

	/** 相机的相对坐标的原点 */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FTransform RelativeOrigin;

	/** 针对现实世界的坐标 */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FTransform RealWorldTransform;

	UPROPERTY()
	FVector OffsetLocation;

	UPROPERTY()
	float OffsetRotation;

public:
	/** 针对现实世界的坐标 */
	FTransform GetRealWorldTransform() const { return RealWorldTransform; }

	/** 旋转操作 */
	void SnapTurn(float Angle);

	/** 跳转到指定位置 */
	void MoveTo(FVector DestLocation);
#pragma endregion
};
