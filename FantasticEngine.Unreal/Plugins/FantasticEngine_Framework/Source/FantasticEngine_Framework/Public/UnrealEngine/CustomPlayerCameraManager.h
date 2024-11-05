// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
// <CustomEngine>
#include "Camera/CameraComponent.h"
// </CustomEngine>
#include "CustomPlayerCameraManager.generated.h"

/**
 * 添加可以显示隐藏的 Actor 的功能，配合 CustomCamera 一起使用
 * https://github.com/EpicGames/UnrealEngine/pull/8896/commits/400abcc895c3d37f2725fdb78e144879bd9b9502
 */
UCLASS(notplaceable, transient, BlueprintType, Blueprintable, Config=Engine)
class FANTASTICENGINE_FRAMEWORK_API ACustomPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()


	// /// <CustomEngine>
	// /** Adding the ability to get current active camera from viewTarget */
	// UFUNCTION(BlueprintCallable, Category = "Util|TeamVskyCustom")
	// virtual const UCameraComponent* GetViewTargetCameraComponent();
	// /// </CustomEngine>
};
