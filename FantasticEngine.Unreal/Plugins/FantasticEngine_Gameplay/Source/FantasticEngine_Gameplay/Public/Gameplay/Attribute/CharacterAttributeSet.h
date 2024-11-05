// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorAttributeSet.h"
#include "UObject/Object.h"
#include "CharacterAttributeSet.generated.h"

/**
 * 属性类
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_GAMEPLAY_API UCharacterAttributeSet : public UActorAttributeSet
{
	GENERATED_BODY()

public:
	/**
	 * 空血判断
	 */
	UFUNCTION(BlueprintPure, Exec, BlueprintNativeEvent, Category="Character|Attributes")
	bool IsEmptyHealth();
};
