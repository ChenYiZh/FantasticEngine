// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/TransparentParameters.h"
#include "UObject/Object.h"
#include "GameEffect.generated.h"

/**
 * 属性计算公式
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASYENGINE_GAMEPLAY_API UGameEffect : public UObject
{
	GENERATED_BODY()
	friend class UGameplayCharacterComponent;
	friend class UGameplayLibrary;

private:
	/**
	 * 是否已经执行过
	 */
	UPROPERTY(VisibleAnywhere, Category="GameEffect|Condition")
	bool bApplied = false;
	/**
	 * 从哪个对象进行发动的技能计算公式
	 */
	UPROPERTY(VisibleAnywhere, Instanced, Category="GameEffect|Context")
	AActor* Source;
	/**
	 * 这个计算公式是施加到哪个对象身上的
	 */
	UPROPERTY(VisibleAnywhere, Instanced, Category="GameEffect|Context")
	AActor* Target;

protected:
	/**
	 * 获取发动的对象
	 */
	UFUNCTION(BlueprintPure, Category="GameEffect|Context")
	AActor* GetSourceAsActor() const;

	/**
	 * 获取发动的角色
	 */
	UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="GameEffect|Context")
	class UGameplayCharacterComponent* GetSourceAsCharacter(
		TSubclassOf<UGameplayCharacterComponent> Class = nullptr) const;

	/**
	 * 获取发动的角色
	 */
	template <typename T>
	T* GetSourceCharacter() const { return Cast<T>(Source); }

	/**
	 * 获取发动角色的属性
	 */
	UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="GameEffect|Context",
		DisplayName="Source Attributes")
	class UAttributeCollection* GetSourceAttributes() const;

	// /**
	//  * 获取发动角色的属性
	//  */
	// template <typename T>
	// T* GetSourceOriAttributes() const { return Cast<T>(GetSourceAttributes()); }
	//
	// /**
	//  * 获取发动角色的策略
	//  */
	// UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="GameEffect|Context",
	// 	DisplayName="Source Strategy")
	// class UActorStrategy* GetSourceOriStrategy(TSubclassOf<UActorStrategy> Class = nullptr) const;

	// /**
	//  * 获取发动角色的策略
	//  */
	// template <typename T>
	// T* GetSourceOriStrategy() const { return Cast<T>(GetSourceOriStrategy()); }


	/**
	 * 获取目标的对象
	 */
	UFUNCTION(BlueprintPure, Category="GameEffect|Context", DisplayName="Target Of Actor")
	AActor* GetTargetAsActor() const;
	/**
	 * 获取目标的角色
	 */
	UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="GameEffect|Context",
		DisplayName="Target Of Character")
	class UGameplayCharacterComponent* GetTargetAsCharacter(
		TSubclassOf<UGameplayCharacterComponent> Class = nullptr) const;
	/**
	 * 获取目标的角色
	 */
	template <typename T>
	T* GetTargetCharacter() const { return Cast<T>(Target); }

	/**
	 * 获取目标角色的属性
	 */
	UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="GameEffect|Context",
		DisplayName="Target Attributes")
	class UAttributeCollection* GetTargetAttributes() const;
	// /**
	//  * 获取目标角色的属性
	//  */
	// template <typename T>
	// T* GetTargetOriAttributes() const { return Cast<T>(GetTargetOriAttributes()); }
	//
	// /**
	//  * 获取目标角色的策略
	//  */
	// UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="GameEffect|Context",
	// 	DisplayName="Target Strategy")
	// class UActorStrategy* GetTargetOriStrategy(TSubclassOf<UActorStrategy> Class = nullptr) const;
	// /**
	//  * 获取目标角色的策略
	//  */
	// template <typename T>
	// T* GetTargetOriStrategy() const { return Cast<T>(GetTargetOriStrategy()); }

protected:
	/**
	 * 攻击计算公式调用
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintNativeEvent, Category="GameEffect")
	void Execute(const int32& TargetIndex,
	             UPARAM(ref) FTransparentParameters& Parameters);

private:
	/**
	 * 应用攻击计算
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param Parameters 透传参数
	 */
	bool OnApply(const int32& TargetIndex, FTransparentParameters& Parameters);

protected:
	/**
	 * 是否可以应用攻击计算
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintNativeEvent, Category="GameEffect")
	bool CanApply(const int32& TargetIndex,UPARAM(ref) FTransparentParameters& Parameters);

public:
	/**
	 * 开始执行攻击计算
	 * @param Ability 从哪个技能发动的攻击
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintCallable, Category="GameEffect")
	void Apply(class UGameModeAbility* Ability,
	           const int32 TargetIndex = 0,
	           FTransparentParameters Parameters = FTransparentParameters());

	/**
	 * 只创建攻击计算类
	 */
	UFUNCTION(BlueprintPure, Category="GameEffect")
	static UGameEffect* MakeGameEffectOnly(const UObject* WorldContextObject, TSubclassOf<UGameEffect> Class,
	                                       AActor* InSource, AActor* InTarget);

	/**
	 * 创建并执行攻击计算
	 * @param Class 攻击计算类
	 * @param InSource 发起者
	 * @param InTarget 目标对象
	 * @param SourceAbility 从哪个技能发动的攻击
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintCallable, Category="GameEffect")
	static UGameEffect* MakeAndApplyGameEffect(const UObject* WorldContextObject,
	                                           TSubclassOf<UGameEffect> Class,
	                                           AActor* InSource,
	                                           AActor* InTarget,
	                                           UGameModeAbility* SourceAbility,
	                                           const int32 TargetIndex = 0,
	                                           FTransparentParameters Parameters = FTransparentParameters());

public:
	/**
	 * 重置状态，可反复使用
	 */
	UFUNCTION(BlueprintCallable, Category="GameEffect")
	void Reset();
};
