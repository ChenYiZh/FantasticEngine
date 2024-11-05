// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VirtualJoystickWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "LeftVirtualJoystickWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_VIRTUALINPUT_API ULeftVirtualJoystickWidget : public UVirtualJoystickWidget
{
	GENERATED_BODY()

public:
	/* 右摇杆是否反向 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs")
	bool bDefaultInvertGamePadRightY = false;
	/* 左摇杆是否锁定摇杆位置 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs")
	bool bLockLeftJoystickPosition;
	// /* 左侧拖拽区域是否只做移动处理 */
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	// bool bLeftAreaExtendLastOperation;
	/* 左摇杆是否常显示，只在锁定位置下有效 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs", meta=(EditCondition="LockLeftJoystickPosition"))
	bool bAwalysDisplayLeftJoystick;
	/* 是否旋转背景 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs")
	bool bRotateLeftJoystickBackgroundOnMoving;
	/* 移动安全区域边框 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs")
	float LeftJoystickMoveBorder;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs")
	FVector2D LeftJoystickOffset = FVector2D::Zero();
	/** 触屏模拟视角调整的缩放 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs")
	FVector2D RightJoystickMoveScale = FVector2D::UnitVector;
	/** 触屏模拟滚轮的缩放 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs")
	float RightJoystickWheelScale = 1.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	float LeftJoystickMoveRange;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	int LeftJoystickTouchEnabled;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	int LeftJoystickTouchIndex;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	FVector2D LeftJoystickDefaultPosition;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	FVector2D LeftJoystickStartPosition;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	UCanvasPanelSlot* LeftJoystickBackgroundSlot;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	UCanvasPanelSlot* LeftJoystickThumbSlot;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Configs|View")
	UWidget* LeftJoystickThumb;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Configs|View")
	UWidget* LeftJoystickBackground;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	bool RightJoystickMoving;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	FInputTouch RightJoystickEvent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	bool RightScaleJoystickMoving;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Configs|Variables")
	FInputTouch RightScaleJoystickEvent;

protected:
	UFUNCTION(BlueprintPure)
	bool GetLeftJoystickAlwaysDisplay() const;
	UFUNCTION(BlueprintPure)
	FVector2D GetLeftJoystickCenterPosition() const;
	UFUNCTION(BlueprintPure)
	FVector2D GetLeftJoystickAnchorPosition() const;

	UFUNCTION(BlueprintPure)
	bool GetRightJoystickEnable() const;

public:
	virtual void OnWidgetOpen_Implementation(UWidgetParam* Param) override;
	virtual void OnTick_Implementation(float DeltaTime) override;

protected:
	virtual void OnTouchDown_Implementation(const FInputTouch& TouchEvent) override;
	virtual void OnTouchMove_Implementation(const FInputTouch& TouchEvent) override;
	virtual void OnTouchUp_Implementation(const FInputTouch& TouchEvent) override;

protected:
	virtual void CheckLeftTouchBegun(const FInputTouch& TouchEvent);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnTouchLeft(const FInputTouch& TouchEvent);
	virtual void OnTouchLeft_Implementation(const FInputTouch& TouchEvent);

	UFUNCTION(BlueprintCallable)
	void LeftControllerAnalog(const FVector2D& LeftOffset);
	UFUNCTION(BlueprintCallable)
	void RightControllerAnalog(const FVector2D& RightOffset);

	UFUNCTION(BlueprintCallable)
	void MouseWheelAnalog(const float Delta);

private:
	FVector2D GetScreenOffset() const;
};
