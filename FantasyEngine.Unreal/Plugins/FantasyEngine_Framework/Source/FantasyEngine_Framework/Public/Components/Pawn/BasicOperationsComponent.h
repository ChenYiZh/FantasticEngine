// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "BasicOperationsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveDynamic, float, Value);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnReceiveNative, float);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveDynamic, float, Value, bool, bImmediately);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMoveNative, float, bool);

USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FBasicPawnSettings
{
	GENERATED_BODY()

public:
	/** 移动计算时是否使用 AddMovementInput ，非 Pawn 为 false */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs|Move",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true"))
	bool bUseMovementInput = true;

	/** 当bUseMovementInput为false时，距离移动的速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs|Move",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true", EditCondition="!bUseMovementInput", EditConditionHides
		))
	float MoveSpeed = 260.f;

	/** 是否禁止上下旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="Configs|LookUp",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true"))
	bool bLockLookUp = false;
	/** 是否禁止左右旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="Configs|LookUp",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true"))
	bool bLockTurn = false;

	/** 是否在LookUp计算时使用 AddControllerRotator ，非 Pawn 为 false */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs|LookUp",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true"))
	bool bUseControllerLookUp = true;
	/** 是否在Turn计算时使用 AddControllerRotator ，非 Pawn 为 false */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs|Turn",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true"))
	bool bUseControllerTurn = true;
	/** 上下旋转视野的比例 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configs|LookUp",
		meta=(DisplayPriority=-1))
	float BaseLookUpRate = 360.f * 4;
	/** 左右旋转视野的比例 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configs|Turn",
		meta=(DisplayPriority=-1))
	float BaseTurnRate = 360.f * 4;

	/** 相机组件 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Camera",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;
};

/** 基础操作类 */
UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UBasicOperationsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBasicOperationsComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables")
	FBasicPawnSettings Settings;

#pragma region 操作管理

public:
	/** 前后操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void MoveForward(float Value, bool bImmediately);

	/** 左右操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void MoveRight(float Value, bool bImmediately);

	/** 视角上下移动操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void LookUp(float Value);

	/** 视角左右旋转操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void Turn(float Value);
#pragma endregion

#pragma region Events

public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Move Forward")
	FOnMoveDynamic K2_OnMoveForward;
	FOnMoveNative OnMoveForward;
	UPROPERTY(BlueprintAssignable, DisplayName="On Move Right")
	FOnMoveDynamic K2_OnMoveRight;
	FOnMoveNative OnMoveRight;
	UPROPERTY(BlueprintAssignable, DisplayName="On Look Up")
	FOnReceiveDynamic K2_OnLookUp;
	FOnReceiveNative OnLookUp;
	UPROPERTY(BlueprintAssignable, DisplayName="On Turn")
	FOnReceiveDynamic K2_OnTurn;
	FOnReceiveNative OnTurn;
#pragma endregion

#pragma region Variables

public:
	/** 获取相机组件 */
	FORCEINLINE UCameraComponent* GetCamera() const { return Settings.Camera; }
#pragma endregion
};
