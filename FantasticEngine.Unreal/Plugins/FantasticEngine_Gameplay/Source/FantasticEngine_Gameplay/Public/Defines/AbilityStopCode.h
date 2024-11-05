// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityStopCode.generated.h"

/**
 * Ability退出的原因
 */
UENUM(BlueprintType)
enum class EAbilityStopCode:uint8
{
	/**
	 * 正常退出
	 */
	Normal = 0,
	/**
	 * 打断
	 */
	Interrupt = 10,
	// /**
	//  * 最后回收时调用
	//  */
	// Destroy = 10,
};
