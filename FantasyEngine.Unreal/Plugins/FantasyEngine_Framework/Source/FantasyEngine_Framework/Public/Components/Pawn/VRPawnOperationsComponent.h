// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "NiagaraComponent.h"
#include "Components/Pawn/BasicOperationsComponent.h"
#include "VRPawnOperationsComponent.generated.h"

/** 跳转的输入状态 */
UENUM(BlueprintType)
enum class ETeleportState:uint8
{
	Started = 0,
	Triggered = 1,
	Completed = 2,
};

/** 跳转的输入状态 */
UENUM(BlueprintType)
enum class EHandGraspState:uint8
{
	StartedOrTriggered = 0,
	Canceled = 1,
	Completed = 2,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportedDynamic, FVector, NewLocation);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeleportedNative, FVector);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGrabbedOrDroppedDynamic, bool, bLeft, class UGrabComponent*, Item);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGrabbedOrDroppedNative, bool, class UGrabComponent*);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMenuTapDynamic, bool, bLeft);

DECLARE_MULTICAST_DELEGATE_OneParam(FMenuTapNative, bool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTapDynamic, bool, bLeft, EHandGraspState, GraspState);

DECLARE_MULTICAST_DELEGATE_TwoParams(FTapNative, bool, EHandGraspState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FValueTapDynamic, bool, bLeft, EHandGraspState, GraspState, float, Value)
;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FValueTapNative, bool, EHandGraspState, float);

USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FVRPawnSettings
{
	GENERATED_BODY()

#pragma region 操作对象

public:
	/** VR的原点 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables|Camera", meta=(AllowPrivateAccess="true"))
	TWeakObjectPtr<USceneComponent> VROrigin;
#pragma endregion

#pragma region 操作管理
#pragma region Turn

public:
	/** 手柄控制的旋转差值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs|Turn")
	float SnapTurnDegrees = 45;
#pragma endregion

#pragma region Teleport

public:
	/** 跳转指示的线特效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables|Teleport", meta=(AllowPrivateAccess="true"))
	TWeakObjectPtr<UNiagaraComponent> TeleportTraceNiagaraSystem = nullptr;

	/** 跳转平台显示对象 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables|Teleport", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> TeleportVisualizerClass = nullptr;

	/** 跳转方向指示对象 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables|Teleport", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> TeleportMotionAim = nullptr;
#pragma endregion

#pragma region Grab

public:
	/** 左手柄 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables|Grab", meta=(AllowPrivateAccess="true"))
	TWeakObjectPtr<UMotionControllerComponent> MotionControllerLeft = nullptr;

	/** 右手柄 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables|Grab", meta=(AllowPrivateAccess="true"))
	TWeakObjectPtr<UMotionControllerComponent> MotionControllerRight = nullptr;
#pragma endregion

#pragma endregion
};

/** VR基础操作类 */
UCLASS(Blueprintable, BlueprintType, ClassGroup="Fantasy Engine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UVRPawnOperationsComponent : public UBasicOperationsComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	UVRPawnOperationsComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables")
	FVRPawnSettings VRSettings;

#pragma region 操作对象

public:
	/** 判断VR原点是否是Root节点 */
	UFUNCTION(BlueprintPure)
	bool VROriginIsRoot() const
	{
		return GetVROrigin() == GetOwner()->GetRootComponent();
	}

	/** 获取VR原点 */
	UFUNCTION(BlueprintPure, DisplayName="VR Origin")
	USceneComponent* GetVROrigin() const
	{
		return VRSettings.VROrigin.IsValid() ? VRSettings.VROrigin.Get() : GetOwner()->GetRootComponent();
	}
#pragma endregion

#pragma region 操作管理

#pragma region Move

public:
	/** 直接跳跃到指定位置 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Move")
	void MoveTo(FVector NewLocation, bool bOnlyRoot = true);

public:
	virtual void MoveForward_Implementation(float Value, bool bImmediately) override;
	virtual void MoveRight_Implementation(float Value, bool bImmediately) override;
	virtual void Turn_Implementation(float Value) override;
	virtual void LookUp_Implementation(float Value) override;
#pragma endregion

#pragma region Turn

public:
	/** VR下旋转操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void SnapTurn(bool bRightTurn);

#pragma endregion

#pragma region Teleport

public:
	/** VR下移动操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void Teleport(ETeleportState TeleportState);

private:
	/** 射线使用到 */
	UPROPERTY(VisibleAnywhere, Category="Teleport")
	TArray<TEnumAsByte<EObjectTypeQuery>> TeleportObjectTypes = {TEnumAsByte<EObjectTypeQuery>(0)}; //World Static

	/** 是否正在按住状态 */
	UPROPERTY(VisibleAnywhere, Category="Teleport")
	bool bTeleportTraceActive = false;

	/** 跳转平台显示对象 */
	UPROPERTY(VisibleAnywhere, Category="Teleport")
	AActor* TeleportVisualizerReference = nullptr;

	/** 射线检测完的着陆点 */
	UPROPERTY(VisibleAnywhere, Category="Teleport")
	FVector ProjectedTeleportLocation;

	/** 是否是有效的着陆点 */
	UPROPERTY(VisibleAnywhere, Category="Teleport")
	bool bValidTeleportLocation = false;

public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Teleported")
	FOnTeleportedDynamic K2_OnTeleported;
	FOnTeleportedNative OnTeleported;
#pragma endregion

#pragma region Grab

public:
	/** VR 下抓取 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Character|Actions")
	void Grab(bool bLeft, bool bStarted, UGrabComponent* MouseGrabComponent = nullptr);

protected:
	UFUNCTION(BlueprintNativeEvent, Category="Character|Actions")
	void GrabOnServer(bool bLeft, bool bStarted, UGrabComponent* MouseGrabComponent = nullptr);

	UFUNCTION(NetMulticast, Reliable, Category="Character|Actions")
	void MulticastOnGrab(bool bLeft, FVector WorldLocation, UGrabComponent* GrabComponent);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Actions")
	void OnGrab(bool bLeft, FVector WorldLocation, UGrabComponent* GrabComponent);

	UFUNCTION(NetMulticast, Reliable, Category="Character|Actions")
	void MulticastRelease(bool bLeft, UGrabComponent* GrabComponent);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Actions")
	void Release(bool bLeft, UGrabComponent* GrabComponent = nullptr);

private:
	/** 射线使用到 */
	UPROPERTY(EditAnywhere, Category="Grab")
	TArray<TEnumAsByte<EObjectTypeQuery>> GrabObjectTypes = {TEnumAsByte<EObjectTypeQuery>(3)}; // Physics Body

private:
	/** 左手柄正在抓取的东西 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grab", meta=(AllowPrivateAccess))
	class UGrabComponent* HeldComponentLeft = nullptr;

	/** 右手柄正在抓取的东西 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grab", meta=(AllowPrivateAccess))
	class UGrabComponent* HeldComponentRight = nullptr;

public:
	UGrabComponent* GetHeldComponentLeft() const { return HeldComponentLeft; }
	UGrabComponent* GetHeldComponentRight() const { return HeldComponentRight; }

public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Grabbed")
	FOnGrabbedOrDroppedDynamic K2_OnGrabbed;
	FOnGrabbedOrDroppedNative OnGrabbed;

	UPROPERTY(BlueprintAssignable, DisplayName="On Dropped")
	FOnGrabbedOrDroppedDynamic K2_OnDropped;
	FOnGrabbedOrDroppedNative OnDropped;
#pragma endregion

#pragma region Menu Toggle

private:
	UPROPERTY(BlueprintAssignable, DisplayName="On Menu Toggle")
	FMenuTapDynamic K2_OnMenuToggle;

public:
	FMenuTapNative OnMenuToggle;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void MenuToggle(bool bLeft);


#pragma endregion

#pragma region Hand Grasp

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void HandGrasp(EHandGraspState GraspState, bool bLeft, float Value);

private:
	UPROPERTY(BlueprintAssignable, DisplayName="On Hand Grasp")
	FValueTapDynamic K2_OnHandGrasp;

public:
	FValueTapNative OnHandGrasp;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void HandIndexCurl(EHandGraspState GraspState, bool bLeft, float Value);

private:
	UPROPERTY(BlueprintAssignable, DisplayName="On Hand Index Curl")
	FValueTapDynamic K2_OnHandIndexCurl;

public:
	FValueTapNative OnHandIndexCurl;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void HandPoint(EHandGraspState GraspState, bool bLeft);

private:
	UPROPERTY(BlueprintAssignable, DisplayName="On Hand Point")
	FTapDynamic K2_OnHandPoint;

public:
	FTapNative OnHandPoint;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void HandThumbUp(EHandGraspState GraspState, bool bLeft);

private:
	UPROPERTY(BlueprintAssignable, DisplayName="On Hand Thumb Up")
	FTapDynamic K2_OnHandThumbUp;

public:
	FTapNative OnHandThumbUp;
#pragma endregion

#pragma endregion
};
