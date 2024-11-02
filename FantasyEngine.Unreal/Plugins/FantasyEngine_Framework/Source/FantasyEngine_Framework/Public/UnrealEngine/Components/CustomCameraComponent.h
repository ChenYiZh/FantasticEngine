// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CustomCameraComponent.generated.h"

/**
 * 添加可以显示隐藏的 Actor 的功能，配合 CustomPlayerCameraManager 一起使用
 * https://github.com/EpicGames/UnrealEngine/pull/8896/commits/400abcc895c3d37f2725fdb78e144879bd9b9502
 */
UCLASS(HideCategories=(Mobility, Rendering, LOD), Blueprintable, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UCustomCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
	// <CustomEngine>
	/**
	 * Adding the ability to have per-camera ignore actor just like ScenceCapture2D
	 */
	// public:
	// UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = CameraOptions)
	// TArray<AActor*> HiddenActors;
	//
	// UFUNCTION(BlueprintCallable, Category = "Util|TeamVskyCustom|Camera")
	// void ClearAllHiddenActorsForCamera();
	//
	// UFUNCTION(BlueprintCallable, Category = "Util|TeamVskyCustom|Camera")
	// void AddHiddenActorToCamera(AActor* toIgnore);
	//
	// UFUNCTION(BlueprintCallable, Category = "Util|TeamVskyCustom|Camera")
	// void RemoveHiddenActorToCamera(AActor* toRemove);

	// </CustomEngine>
};
