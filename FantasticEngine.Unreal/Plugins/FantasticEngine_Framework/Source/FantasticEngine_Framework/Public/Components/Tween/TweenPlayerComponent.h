// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TweenPlayerComponent.generated.h"


/** 控制子组件的操作 */
UCLASS(NotBlueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UTweenPlayerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UTweenPlayerComponent();

public:
	/** 正向播放所有子动画 */
	UFUNCTION(BlueprintCallable, Category="Tween")
	void PlayAllForward();
	/** 反向播放所有子动画 */
	UFUNCTION(BlueprintCallable, Category="Tween")
	void PlayAllBack();
	/** 重置所有子动画到正向起始状态 */
	UFUNCTION(BlueprintCallable, Category="Tween")
	void ResetAllToBegin();
	/** 重置所有子动画到反向起始状态 */
	UFUNCTION(BlueprintCallable, Category="Tween")
	void ResetAllToEnd();
	/** 暂停所有子动画 */
	UFUNCTION(BlueprintCallable, Category="Tween")
	void PauseAll();
	/** 播放所有子动画 */
	UFUNCTION(BlueprintCallable, Category="Tween")
	void ResumeAll();
	/** 停止所有子动画 */
	UFUNCTION(BlueprintCallable, Category="Tween")
	void StopAll();
};
