// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LeftVirtualJoystickWidget.h"
#include "MobileGameVirtualJoystickWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_VIRTUALINPUT_API UMobileGameVirtualJoystickWidget : public ULeftVirtualJoystickWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceUpPressed();
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceUpReleased();
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceDownPressed();
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceDownReleased();
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceLeftPressed();
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceLeftReleased();
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceRightPressed();
	UFUNCTION(BlueprintCallable)
	void InputGamepadFaceRightReleased();
};
