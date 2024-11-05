// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeAbility.h"
#include "Gameplay/Task/GameplayTask_PlayMontage.h"
#include "UObject/Object.h"
#include "GameModeAbility_Montage.generated.h"

/**
 * 蒙太奇技能管理类
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_GAMEPLAY_API UGameModeAbility_Montage : public UGameModeAbility
{
	GENERATED_BODY()

public:
	/**
	 * 执行蒙太奇动画的通知操作
	 */
	virtual void ExecuteMontageBegin(UAnimMontage* MontagePlaying);
	/**
	 * 执行蒙太奇动画的通知操作
	 */
	virtual void ExecuteMontageEnd(UAnimMontage* MontagePlaying);
	/**
	 * 执行蒙太奇动画的通知操作
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnMontageBegin(UAnimMontage* MontagePlaying);
	/**
	 * 执行蒙太奇动画的通知操作
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnMontageEnd(UAnimMontage* MontagePlaying);
	/**
	 * 执行通知操作
	 */
	virtual void ExecuteNofityBegin(UAnimMontage* MontagePlaying, FName Name, const FTransparentParameters& Parameters);
	/**
	 * 执行通知操作
	 */
	virtual void ExecuteNofityEnd(UAnimMontage* MontagePlaying, FName Name, const FTransparentParameters& Parameters);
	/**
	 * 执行通知操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Ability")
	void OnNofityBegin(UAnimMontage* MontagePlaying, FName Name, const FTransparentParameters& Parameters);
	/**
	 * 执行通知操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Ability")
	void OnNofityEnd(UAnimMontage* MontagePlaying, FName Name, const FTransparentParameters& Parameters);
};
