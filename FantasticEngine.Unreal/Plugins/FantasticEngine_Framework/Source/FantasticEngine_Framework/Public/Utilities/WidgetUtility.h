// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetUtility.generated.h"

/** UI辅助类 */
UCLASS()
class FANTASTICENGINE_FRAMEWORK_API UWidgetUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 设置Slider的鼠标是否使用步长 */
	UFUNCTION(BlueprintCallable, Category="Widget Utility|Slider")
	static void SetSliderMouseUsesStep(USlider* Slider, bool UsesStep);
};
