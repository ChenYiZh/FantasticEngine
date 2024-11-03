// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/DefaultInputDefines.h"
#include "Defines/InputDefines.h"
#include "UObject/Object.h"
#include "GameplayInputDefines.generated.h"

/**
 * 输入管理类，标准的输入定义
 */
UCLASS()
class FANTASYENGINE_GAMEPLAY_API UGameplayInputDefines : public UDefaultInputDefines
{
	GENERATED_BODY()

#pragma region 手柄左边的上下左右键
private:
	/**
	 * 左上键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Arrow")
	TObjectPtr<UInputAction> IA_ArrowUp;
	/**
	 * 左下键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Arrow")
	TObjectPtr<UInputAction> IA_ArrowDown;
	/**
	 * 左左键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Arrow")
	TObjectPtr<UInputAction> IA_ArrowLeft;
	/**
	 * 左右键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Arrow")
	TObjectPtr<UInputAction> IA_ArrowRight;
public:
	/**
	 * 左上键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Arrow", DisplayName="Arrow Up")
	UInputAction* GetIA_ArrowUp() const;
	/**
	 * 左下键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Arrow", DisplayName="Arrow Down")
	UInputAction* GetIA_ArrowDown() const;
	/**
	 * 左左键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Arrow", DisplayName="Arrow Left")
	UInputAction* GetIA_ArrowLeft() const;
	/**
	 * 左右键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Arrow", DisplayName="Arrow Right")
	UInputAction* GetIA_ArrowRight() const;
#pragma endregion

#pragma region 手柄右边的Action键
private:
	/**
	 * 右上键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Action")
	TObjectPtr<UInputAction> IA_ActionUp;
	/**
	 * 右下键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Action")
	TObjectPtr<UInputAction> IA_ActionDown;
	/**
	 * 右左键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Action")
	TObjectPtr<UInputAction> IA_ActionLeft;
	/**
	 * 右右键
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input|Action")
	TObjectPtr<UInputAction> IA_ActionRight;
public:
	/**
	 * 右上键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Action", DisplayName="Action Up")
	UInputAction* GetIA_ActionUp() const;
	/**
	 * 右上键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Action", DisplayName="Action Down")
	UInputAction* GetIA_ActionDown() const;
	/**
	 * 右上键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Action", DisplayName="Action Left")
	UInputAction* GetIA_ActionLeft() const;
	/**
	 * 右上键
	 */
	UFUNCTION(BlueprintPure, Category="Input Action|Action", DisplayName="Action Right")
	UInputAction* GetIA_ActionRight() const;
#pragma endregion
};
