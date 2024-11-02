// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/InputDefines.h"
#include "VRInputDefines.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYENGINE_FRAMEWORK_API UVRInputDefines : public UInputDefines
{
	GENERATED_BODY()

#pragma region 手柄输入

private:
	/** VR下位移按键 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Default")
	TObjectPtr<UInputAction> IA_VRMove;
	/** VR下旋转按键 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Default")
	TObjectPtr<UInputAction> IA_VRTurn;
	/** 左手抓取按键 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Default")
	TObjectPtr<UInputAction> IA_Grab_Left;
	/** 右手抓取按键 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Default")
	TObjectPtr<UInputAction> IA_Grab_Right;

	/** 左手菜单按键 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Default")
	TObjectPtr<UInputAction> IA_Menu_Toggle_Left;
	/** 右手菜单按键 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Default")
	TObjectPtr<UInputAction> IA_Menu_Toggle_Right;

public:
	/** VR下位移按键 */
	UFUNCTION(BlueprintPure, Category="Input Action|Default", DisplayName="VR Move")
	UInputAction* GetIA_VRMove() const
	{
		return IA_VRMove;
	}

	/** VR下旋转按键 */
	UFUNCTION(BlueprintPure, Category="Input Action|Default", DisplayName="VR Turn")
	UInputAction* GetIA_VRTurn() const
	{
		return IA_VRTurn;
	}

	/** 左手抓取按键 */
	UFUNCTION(BlueprintPure, Category="Input Action|Default", DisplayName="Grab Left")
	UInputAction* GetIA_Grab_Left() const
	{
		return IA_Grab_Left;
	}

	/** 右手抓取按键 */
	UFUNCTION(BlueprintPure, Category="Input Action|Default", DisplayName="Grab Right")
	UInputAction* GetIA_Grab_Right() const
	{
		return IA_Grab_Right;
	}

	/** 左手菜单按键 */
	UFUNCTION(BlueprintPure, Category="Input Action|Default", DisplayName="Menu Toggle Left")
	UInputAction* GetIA_Menu_Toggle_Left() const
	{
		return IA_Menu_Toggle_Left;
	}

	/** 右手菜单按键 */
	UFUNCTION(BlueprintPure, Category="Input Action|Default", DisplayName="Menu Toggle Right")
	UInputAction* GetIA_Menu_Toggle_Right() const
	{
		return IA_Menu_Toggle_Right;
	}
#pragma endregion

#pragma region 手势输入

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_Point_Left;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_Point_Right;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_ThumbUp_Left;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_ThumbUp_Right;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_Grasp_Left;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_Grasp_Right;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_IndexCurl_Left;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Hand")
	TObjectPtr<UInputAction> IA_Hand_IndexCurl_Right;

public:
	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand Point Left")
	UInputAction* GetIA_Hand_Point_Left() const
	{
		return IA_Hand_Point_Left;
	}

	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand Point Right")
	UInputAction* GetIA_Hand_Point_Right() const
	{
		return IA_Hand_Point_Right;
	}

	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand ThumbUp Left")
	UInputAction* GetIA_Hand_ThumbUp_Left() const
	{
		return IA_Hand_ThumbUp_Left;
	}

	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand ThumbUp Right")
	UInputAction* GetIA_Hand_ThumbUp_Right() const
	{
		return IA_Hand_ThumbUp_Right;
	}

	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand Grasp Left")
	UInputAction* GetIA_Hand_Grasp_Left() const
	{
		return IA_Hand_Grasp_Left;
	}

	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand Grasp Right")
	UInputAction* GetIA_Hand_Grasp_Right() const
	{
		return IA_Hand_Grasp_Right;
	}

	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand IndexCurl Left")
	UInputAction* GetIA_Hand_IndexCurl_Left() const
	{
		return IA_Hand_IndexCurl_Left;
	}

	UFUNCTION(BlueprintPure, Category="Input Action|Hand", DisplayName="Hand IndexCurl Right")
	UInputAction* GetIA_Hand_IndexCurl_Right() const
	{
		return IA_Hand_IndexCurl_Right;
	}
#pragma endregion
};
