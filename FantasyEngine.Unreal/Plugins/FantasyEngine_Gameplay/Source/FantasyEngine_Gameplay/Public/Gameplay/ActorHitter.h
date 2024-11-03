// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackContext.h"
#include "UObject/Interface.h"
#include "ActorHitter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorHitter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 受击事件
 */
class FANTASYENGINE_GAMEPLAY_API IActorHitter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * 受击时调用，需要调用父函数，不然需要自己判断空血状态
	 * @param Context 受击传入的攻击上下文
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Hit(const FAttackContext& Context);
};
