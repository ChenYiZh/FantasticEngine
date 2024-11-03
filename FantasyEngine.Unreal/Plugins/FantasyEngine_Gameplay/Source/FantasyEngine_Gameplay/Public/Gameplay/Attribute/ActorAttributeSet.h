// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActorAttributeSet.generated.h"

/**
 * 属性类
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASYENGINE_GAMEPLAY_API UActorAttributeSet : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 重置操作
	 */
	UFUNCTION(BlueprintCallable, Exec, BlueprintNativeEvent, Category="Actor|Attributes")
	void Reset();
};
