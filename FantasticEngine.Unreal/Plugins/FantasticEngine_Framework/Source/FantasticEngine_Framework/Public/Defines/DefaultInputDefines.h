// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/InputDefines.h"
#include "DefaultInputDefines.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_FRAMEWORK_API UDefaultInputDefines : public UInputDefines
{
	GENERATED_BODY()

#pragma region Move
private:
	/**
	 * 移动映射
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Move")
	TObjectPtr<UInputAction> IA_MoveForward;
	/**
	 * 移动映射
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Move")
	TObjectPtr<UInputAction> IA_MoveRight;

public:
	/**
	 * 纵向移动行为
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Move", DisplayName="Move Forward")
	UInputAction* GetIA_MoveForward() const
	{
		return IA_MoveForward;
	}

	/**
	 * 横向移动行为
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Move", DisplayName="Move Right")
	UInputAction* GetIA_MoveRight() const
	{
		return IA_MoveRight;
	}
#pragma endregion

#pragma region Turn

private:
	/**
	 * 视角映射
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Turn")
	TObjectPtr<UInputAction> IA_LookUp;
	/**
	 * 视角映射
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Turn")
	TObjectPtr<UInputAction> IA_Turn;

public:
	/**
	 * 视角上移行为
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Turn", DisplayName="Look Up")
	UInputAction* GetIA_LookUp() const
	{
		return IA_LookUp;
	}

	/**
	 * 视角旋转行为
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Turn", DisplayName="Turn")
	UInputAction* GetIA_Turn() const
	{
		return IA_Turn;
	}
#pragma endregion

#pragma region ViewLength

private:
	/**
	 * 视线距离映射
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|View")
	TObjectPtr<UInputAction> IA_ViewClose;

public:
	/**
	 * 视线距离调整行为
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|View", DisplayName="View Close")
	UInputAction* GetIA_ViewClose() const
	{
		return IA_ViewClose;
	}
#pragma endregion

#pragma region 鼠标控制

private:
	/**
	 * 鼠标控制
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Mouse")
	TObjectPtr<UInputAction> IA_MouseControl;

public:
	/**
	 * 鼠标控制
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Mouse", DisplayName="Mouse Control")
	UInputAction* GetIA_MouseControl() const
	{
		return IA_MouseControl;
	}
#pragma endregion
};
