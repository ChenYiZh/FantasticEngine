// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "Defines/EnumDefines.h"
#include "InputSystem.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UInputSystem : public USystemBasis
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FDateTime LastTapTime;

	UPROPERTY()
	bool bCursorVisible = false;

public:
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Input System", meta=(WorldContext="WorldContextObject"))
	static bool CanTap(const UObject* WorldContextObject, bool bRefresh = true);
	/**
	 * 设置鼠标显示类型
	 * @param NewMouseType 新的鼠标显示类型
	 * @param bChangeCache 是否修改缓存值
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Input System", meta=(WorldContext="WorldContextObject"))
	static void SetMouseType(const UObject* WorldContextObject, EMouseType NewMouseType,
	                         const bool bChangeCache = false);
	/**
	 * 是否显示Cursor
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Input System", meta=(WorldContext="WorldContextObject"))
	static void SetCursorVisible(const UObject* WorldContextObject, const bool bVisible);

	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Input System", meta=(WorldContext="WorldContextObject"))
	static bool IsCursorVisible(const UObject* WorldContextObject);
};
