// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameplayCharacterComponent.h"
#include "Components/GameplayPlayerControllerComponent.h"
#include "UObject/Object.h"
#include "GameplayLibrary.generated.h"

/**
 * 主玩家管理类
 */
UCLASS()
class FANTASYENGINE_GAMEPLAY_API UGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 获取本地的角色类
	 */
	UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="Game Mode")
	static UGameplayCharacterComponent* GetLocalCharacter(const UObject* WorldContextObject,
	                                                      TSubclassOf<UGameplayCharacterComponent> Class = nullptr,
	                                                      const int32 ControllerId = 0);
	/**
	 * 获取本地的角色类
	 */
	template <typename T>
	static T* GetCharacter(const UObject* WorldContextObject, const int32 ControllerId = 0)
	{
		return Cast<T>(GetLocalCharacter(WorldContextObject, UGameplayCharacterComponent::StaticClass(), ControllerId));
	}

	/**
	 * 获取本地的角色控制类
	 */
	UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="Game Mode")
	static UGameplayPlayerControllerComponent* GetLocalController(
		const UObject* WorldContextObject,
		TSubclassOf<UGameplayPlayerControllerComponent> Class = nullptr,
		const int32 ControllerId = 0);
	/**
	 * 获取本地的角色控制类
	 */
	template <typename T>
	static T* GetController(const UObject* WorldContextObject, const int32 ControllerId = 0)
	{
		return Cast<T>(GetLocalController(WorldContextObject, UGameplayPlayerControllerComponent::StaticClass(),
		                                  ControllerId));
	}

	// /**
	//  * 获取本地的角色策略类
	//  */
	// UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="Game Mode")
	// static UActorStrategy* GetLocalStrategy(
	// 	TSubclassOf<UActorStrategy> Class = nullptr,
	// 	const int32 ControllerId = 0);
	// /**
	//  * 获取本地的角色策略类
	//  */
	// template <typename T>
	// static T* GetStrategy(const int32 ControllerId = 0)
	// {
	// 	return Cast<T>(GetLocalStrategy(UActorStrategy::StaticClass(), ControllerId));
	// }

	/**
	 * 构造函数
	 * @param Source 发起者
	 * @param GameEffect 攻击的计算公式
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param SourceAbility 从哪个技能发动的攻击
	 * @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintPure, Category="Attack Context")
	static FAttackContext MakeContext(
		AActor* Source,
		UGameEffect* GameEffect,
		const int32 TargetIndex = 0,
		UGameModeAbility* SourceAbility = nullptr,
		FTransparentParameters Parameters = FTransparentParameters());
};
