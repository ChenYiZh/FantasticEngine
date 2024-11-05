// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DieCode.generated.h"

/**
 * 死亡原因
 */
UENUM(BlueprintType)
enum class EDieCode:uint8
{
	/**
	 * 默认原因
	 */
	None = 0,
	/**
	 * 空血
	 */
	EmptyHealth=5,
	/**
	 * 销毁时执行
	 */
	Destroy=10
};
