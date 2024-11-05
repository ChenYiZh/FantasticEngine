// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeAbility_Montage.h"
#include "UObject/Object.h"
#include "GameModeAbility_SingleMontage.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_GAMEPLAY_API UGameModeAbility_SingleMontage : public UGameModeAbility_Montage
{
	GENERATED_BODY()

private:
	/**
	 * 是否在Active时自动播放动画
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Montage")
	bool bAutoPlayOnActive = true;

protected:
	/**
	 * 使用的蒙太奇动画
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability|Montage")
	UAnimMontage* MontageToPlay;
	/**
	 * 在蒙太奇播放完成后，是否直接执行技能完成操作
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability|Montage")
	bool bAutoFinishOnMontageCompleted = true;
	/**
	 * 蒙太奇播放的起始Section
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability|Montage")
	FName StartSection = NAME_None;
	/**
	 * 蒙太奇动画播放速率
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability|Montage")
	float MontagePlayRate = 1.f;
	/**
	 * 蒙太奇动画的起始时间
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability|Montage")
	float StartTimeSeconds = 0.f;
	/**
	 * 在Ability结束后是否同时结束动画
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability|Montage")
	bool bStopWhenAbilityEnds = true;

protected:
	/**
	 * 激活时执行
	 */
	virtual void OnActive() override;
	/**
	 * 播放完成时调用
	 */
	virtual void OnMontageEnd_Implementation(UAnimMontage* MontagePlaying) override;

public:
	/**
	 * 获取蒙太奇对象
	 */
	UAnimMontage* GetMontageToPlay() const;
};
