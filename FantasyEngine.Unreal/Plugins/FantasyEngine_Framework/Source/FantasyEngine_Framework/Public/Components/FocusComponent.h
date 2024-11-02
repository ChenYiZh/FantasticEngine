// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FocusComponent.generated.h"

/** 注视时调用的函数，一般用于行为层
 * @param Target 目标，空为取消注视
 * @param bFocus 是否调用注视
 * @param bKeep 是否一直保持指向
 * @param bInUseTurn 是否使用转身过渡
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLookAtDynamic, USceneComponent*, TargetComponent, bool, bFocus, bool,
                                              bKeep, bool,
                                              bUseTurn);

/** 注视时调用的函数，一般用于行为层
 * @param Target 目标，空为取消注视
 * @param bFocus 是否调用注视
 * @param bKeep 是否一直保持指向
 * @param bInUseTurn 是否使用转身过渡
 */
DECLARE_MULTICAST_DELEGATE_FourParams(FOnLookAtNative, USceneComponent*, bool, bool, bool);

/**
 * 聚焦事件，一般用于表现层
 * @param Target 目标，空为取消聚焦
 * @param bKeep 是否一直保持指向
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFocusDynamic, USceneComponent*, TargetComponent, bool, bKeep);

/**
 * 聚焦事件，一般用于表现层
 * @param Target 目标，空为取消聚焦
 * @param bKeep 是否一直保持指向
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFocusNative, USceneComponent*, bool);

/** 让父节点看向某个位置 */
UCLASS(NotBlueprintable, BlueprintType, ClassGroup="Fantasy Engine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UFocusComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFocusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma region 主要函数

public:
	/** 重置参数 */
	UFUNCTION(BlueprintCallable, Category="Focus")
	void Reset();
	/**
	 * 转向并注视目标
	 * @param TargetComponent 目标
	 * @param bFocus 是否调用注视事件
	 * @param bKeep 是否一直保持指向
	 * @param bInUseTurn 是否使用转身过渡
	 */
	UFUNCTION(BlueprintCallable, Category="Focus")
	void LookAt(USceneComponent* TargetComponent, const bool bFocus = true, const bool bKeep = false,
	            const bool bInUseTurn = false);

private:
	/** 实现函数 */
	void LookAtImpl(USceneComponent* TargetComponent, bool bFocus, bool bKeep, const bool bInUseTurn,
	                const float DeltaSeconds);

protected:
	/** 获取这个组件控制的对象 */
	UFUNCTION(BlueprintPure, Category="Focus")
	USceneComponent* GetControlledTarget();
#pragma endregion


#pragma region Variables

public:
	/** 是否之转动Z轴 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category = "Focus")
	bool bOnlyAxisZ = false;

private:
	/** 在使用转身过度聚焦时的转身速度 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category = "Focus")
	float TurnSpeed = 5.f;

	/** 是否使用转身过渡 */
	UPROPERTY(VisibleAnywhere, Category = "Focus", meta=(DisplayPriority=-1))
	bool bUseTurn = false;

	/** 父节点可能会发生变化，所以需要记录下物体的上一帧角度 */
	FRotator LastWorldRotator;

protected:
	/** 持续面对对象 */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Focus", meta=(DisplayPriority=-1))
	bool bKeepLookAt = false;

	/** 聚焦的对象 */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Focus", DisplayName="Target",
		meta=(DisplayPriority=-1))
	USceneComponent* TargetLookedAt = nullptr;

private:
	/** 注视时调用的函数，一般用于行为层
	 * @param Target 目标，空为取消注视
	 * @param bFocus 是否调用注视
	 * @param bKeep 是否一直保持指向
	 * @param bInUseTurn 是否使用转身过渡
	 */
	UPROPERTY(BlueprintAssignable, DisplayName="On Look At")
	FOnLookAtDynamic K2_OnLookAt;

public :
	/** 注视时调用的函数，一般用于行为层
	 * @param Target 目标，空为取消注视
	 * @param bFocus 是否调用注视
	 * @param bKeep 是否一直保持指向
	 * @param bInUseTurn 是否使用转身过渡
	 */
	FOnLookAtNative OnLookAt;

private:
	/**
	 * 聚焦事件，一般用于表现层
	 * @param Target 目标，空为取消聚焦
	 * @param bKeep 是否一直保持指向
	 */
	UPROPERTY(BlueprintAssignable, DisplayName="On Focus")
	FOnFocusDynamic K2_OnFocus;

public :
	/**
	 * 聚焦事件，一般用于表现层
	 * @param Target 目标，空为取消聚焦
	 * @param bKeep 是否一直保持指向
	 */
	FOnFocusNative OnFocus;
#pragma endregion
};
