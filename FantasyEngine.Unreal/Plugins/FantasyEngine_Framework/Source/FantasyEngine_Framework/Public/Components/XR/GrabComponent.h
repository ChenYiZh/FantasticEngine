// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "Components/SceneComponent.h"
#include "GrabComponent.generated.h"

/** 抓取的类型 */
UENUM(BlueprintType)
enum class EGrabType:uint8
{
	None = 0,
	Free = 1,
	Snap = 2,
	Custom = 3
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabEventDynamic);

DECLARE_MULTICAST_DELEGATE(FGrabEventNative);

UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** 用鼠标来模拟右手 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Grab")
	bool bMouseSimulateRightHand = true;

#pragma region Functions

public:
	/**
	 * @return IsHeld
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Grab")
	bool TryGrab(UMotionControllerComponent* MotionController, bool bForce = false);

	/**
	 * @return Released
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Grab")
	bool TryRelease();

	/** 是否启用物理模拟 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Grab")
	void SetPrimitiveCompPhysics(bool bSimulate);

protected:
	/** 附加到MotionController上 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Grab")
	bool AttachParentToMotionController(UMotionControllerComponent* MotionController);
#pragma endregion

#pragma region Variables

public:
	/** 抓取的类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grab")
	EGrabType GrabType = EGrabType::Free;

private:
	/** 是否启用下落时的物理模拟 */
	UPROPERTY(EditAnywhere, Category="Grab")
	bool bSimulateOnDrop = false;
	/** 抓取的类型 */
	UPROPERTY(EditAnywhere, Category="Grab", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UHapticFeedbackEffect_Base> OnGrabHapticEffect;

	/** 被谁抓取着 */
	UPROPERTY(VisibleAnywhere, Category="Grab")
	UMotionControllerComponent* MotionControllerRef = nullptr;

	/** 是否是在抓取状态 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grab", meta=(AllowPrivateAccess))
	bool bIsHeld = false;
#pragma endregion

#pragma region Events

private:
	UPROPERTY(BlueprintAssignable, DisplayName="On Grabbed")
	FGrabEventDynamic K2_OnGrabbed;

	UPROPERTY(BlueprintAssignable, DisplayName="On Dropped")
	FGrabEventDynamic K2_OnDropped;

public:
	FGrabEventNative OnGrabbed;
	FGrabEventNative OnDropped;
#pragma endregion

#pragma region Mouse

private:
	FVector StartLocation;
	FVector StartMousePosition;
	FVector StartMouseDirection;
	bool bMouseDown = false;
	bool bLastMouseDown = false;

	bool bHover = false;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginMouseHover(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintNativeEvent)
	void OnEndMouseHover(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintNativeEvent)
	void OnMousePressed(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION(BlueprintNativeEvent)
	void OnMouseReleased(UPrimitiveComponent* TouchedComponent, FKey ButtonReleased);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region State

private:
	bool bLastHeld = false;
#pragma endregion
};
