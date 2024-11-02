// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimitivePlayerControllerComponent.h"
#include "AdvancedPlayerControllerComponent.generated.h"

/** 视野模式 */
UENUM(BlueprintType)
enum class EGameViewMode:uint8
{
	/** 默认3d视角 */
	V_3D = 0,
	/** 2d视角操作 */
	V_2D = 2,
	/** 2.5d视角操作 */
	V_25D = 25
};

/** 快速响应方向 */
UENUM(BlueprintType, meta=(Bitflags))
enum class EQuickDirection:uint8
{
	/** 左右方向快速变化 */
	LeftAndRight = 0,
	/** 前后方向快速变化 */
	ForwardAndBack = 1,
	/** 上下方向快速变化 */
	UpAndDown = 2,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnViewDynamic, FVector2D, Value);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTurnViewNative, FVector2D);

UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UAdvancedPlayerControllerComponent : public UPrimitivePlayerControllerComponent
{
	GENERATED_BODY()

#pragma region 操作管理

public:
	virtual void MoveForward_Implementation(const FInputActionInstance& Instance) override;

	virtual void MoveRight_Implementation(const FInputActionInstance& Instance) override;

	virtual void LookUp_Implementation(const FInputActionInstance& Instance) override;

	virtual void Turn_Implementation(const FInputActionInstance& Instance) override;

public:
	/** 非3D模式下的视角调整 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions|2D")
	void TurnView(const FVector2D Value);
#pragma endregion

#pragma region 默认角色操作类

protected:
	/** 游戏视角 */
	UPROPERTY(EditAnywhere, Category="Contoller|View")
	EGameViewMode ViewMode = EGameViewMode::V_3D;

public:
	/** 视角是否需要锁定纵深方向 */
	UPROPERTY(EditAnywhere, Category="Contoller|View")
	bool bLockAxisZ = false;
	/** 快速旋转的方向 */
	UPROPERTY(EditAnywhere, Category="Contoller|View", meta=(Bitmask, BitmaskEnum="/Script/Framework.EQuickDirection"))
	int32 QuickDirection = 0;

public:
	/** 非3D模式下的视角调整回调 */
	UPROPERTY(BlueprintAssignable, DisplayName="On Turn View")
	FOnTurnViewDynamic K2_OnTurnView;

	/** 非3D模式下的视角调整回调 */
	FOnTurnViewNative OnTurnView;
#pragma endregion
};
