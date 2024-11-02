#pragma once

#include "CoreMinimal.h"
#include "Basis/AnimConfig.h"
#include "Components/Widget.h"

#include "GraphicUtility.generated.h"

UCLASS()
class FANTASYENGINE_FRAMEWORK_API UGraphicUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 获取存储类别名称 **/
	UFUNCTION(BlueprintPure, Exec, Category="Graphic")
	static FString GetSectionName();

public:
	/** 获取显示模式 **/
	UFUNCTION(BlueprintPure, Exec, Category="Graphic")
	static EWindowMode::Type GetWindowMode();

	/** 设置显示模式 **/
	UFUNCTION(BlueprintCallable, Category="Graphic")
	static EWindowMode::Type SetWindowMode(EWindowMode::Type NewWindowMode);

	/** 获取UI尺寸的大小 **/
	UFUNCTION(BlueprintPure, Exec, Category="Graphic", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetViewportSize(const UObject* WorldContextObject);

	/** 获取UI显示比例 **/
	UFUNCTION(BlueprintPure, Exec, Category="Graphic", meta=(WorldContext="WorldContextObject"))
	static float GetViewportScale(const UObject* WorldContextObject);

	/** 获取屏幕大小 **/
	UFUNCTION(BlueprintPure, Exec, Category="Graphic", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetScreenSize(const UObject* WorldContextObject);

	/** 判断是否是刘海屏 **/
	UFUNCTION(BlueprintPure, Exec, Category="Graphic", meta=(WorldContext="WorldContextObject"))
	static bool IsHairScreen(const UObject* WorldContextObject);
	/** 判断是否是带鱼屏 **/
	UFUNCTION(BlueprintPure, Exec, Category="Graphic", meta=(WorldContext="WorldContextObject"))
	static bool IsWideScreen(const UObject* WorldContextObject);

	/** 动画播放逻辑 **/
	UFUNCTION(BlueprintCallable, Category="Display")
	static void PlayUIAnim(UWidget* Widget, EUIAnimType AnimType, bool bInvert, float Alpha);

	/** 适配显示比例 **/
	UFUNCTION(BlueprintPure, Category="Display|Widget", meta=(WorldContext="WorldContextObject"))
	static FVector2D FillScreenWithOriginRate(const UObject* WorldContextObject, const FVector2D InSize);

	/** 判断是否是显示状态 **/
	UFUNCTION(BlueprintPure, Category="Display|Widget")
	static bool IsVisible(ESlateVisibility Visibility);

	/** 判断是否在屏幕内 */
	UFUNCTION(BlueprintPure, Category="Display", meta=(WorldContext="WorldContextObject"))
	static bool InScreen(const UObject* WorldContextObject, FVector WorldLocation, bool bOnlyProjection = false);
};
