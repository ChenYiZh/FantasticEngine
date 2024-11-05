// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/TransparentParameters.h"
#include "AttackContext.generated.h"
/**
 * 攻击上下文
 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_GAMEPLAY_API FAttackContext
{
	GENERATED_BODY()
public:
	/**
	 * 从哪个Actor发起的攻击
	 */
	UPROPERTY(BlueprintReadWrite)
	AActor* Source = nullptr;
	/**
	 * 从哪个技能发动的攻击
	 */
	UPROPERTY(BlueprintReadWrite)
	class UGameModeAbility* SourceAbility = nullptr;
	/**
	 * 计算公式传入
	 */
	UPROPERTY(BlueprintReadWrite)
	class UGameEffect* GameEffect = nullptr;
	/**
	 * 受击对象是受击队列中的第几个
	 */
	UPROPERTY(BlueprintReadWrite)
	int32 HitIndex = 0;
	/**
	 * 透传参数
	 */
	UPROPERTY(BlueprintReadWrite)
	FTransparentParameters Parameters;
};
