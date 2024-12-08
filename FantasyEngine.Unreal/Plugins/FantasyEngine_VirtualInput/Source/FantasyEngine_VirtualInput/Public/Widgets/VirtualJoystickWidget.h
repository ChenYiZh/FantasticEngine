// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/BaseWidget.h"
#include "Basis/BaseWidget.h"
#include "Core/InputDragHandle.h"
#include "Blueprint/UserWidget.h"
#include "VirtualJoystickWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASYENGINE_VIRTUALINPUT_API UVirtualJoystickWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	EMouseLockMode LockMode;
	UPROPERTY(EditAnywhere)
	bool bCursorDisplay;
	/**
	 * 在关闭窗口虚拟监听时鼠标转换
	 */
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Configs|Cursor")
	bool bShowOnClose = true;
	/**
	 * 绑定的按键事件
	 */
	UPROPERTY()
	TArray<int32> InputHandles;
public:
	virtual void NativeConstruct() override;

public:
	virtual void OnWidgetOpen_Implementation(UWidgetParam* Param) override;

	virtual void OnWidgetClosed_Implementation() override;

private:
	void OnCtrlPressed();

	void OnCtrlReleased();

public:
	UFUNCTION()
	void OnTouchDownDelegate(const FInputTouch& TouchEvent);
	UFUNCTION()
	void OnTouchMoveDelegate(const FInputTouch& TouchEvent);
	UFUNCTION()
	void OnTouchUpDelegate(const FInputTouch& TouchEvent);

protected:
	UFUNCTION(BlueprintNativeEvent, Category="Input System")
	void OnTouchDown(const FInputTouch& TouchEvent);
	virtual void OnTouchDown_Implementation(const FInputTouch& TouchEvent);
	UFUNCTION(BlueprintNativeEvent, Category="Input System")
	void OnTouchMove(const FInputTouch& TouchEvent);
	virtual void OnTouchMove_Implementation(const FInputTouch& TouchEvent);
	UFUNCTION(BlueprintNativeEvent, Category="Input System")
	void OnTouchUp(const FInputTouch& TouchEvent);
	virtual void OnTouchUp_Implementation(const FInputTouch& TouchEvent);
};
