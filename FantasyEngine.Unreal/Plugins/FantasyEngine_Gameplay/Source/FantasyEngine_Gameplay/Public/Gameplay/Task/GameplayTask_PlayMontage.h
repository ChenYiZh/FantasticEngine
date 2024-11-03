// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "Defines/TransparentParameters.h"
#include "UObject/Object.h"
#include "GameplayTask_PlayMontage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameplayTaskMontagePlayDelegate, FName, NotifyName,
                                             const FTransparentParameters&, Parameters);

/**
 * 任务计划中的蒙太奇播放
 */
UCLASS()
class FANTASYENGINE_GAMEPLAY_API UGameplayTask_PlayMontage : public UGameplayTask
{
	GENERATED_BODY()
#pragma region 事件注册
public:
	/**
	 * 动画完成时调用
	 */
	UPROPERTY(BlueprintAssignable)
	FGameplayTaskMontagePlayDelegate OnCompleted;
	/**
	 * 被打断时调用
	 */
	UPROPERTY(BlueprintAssignable)
	FGameplayTaskMontagePlayDelegate OnInterrupted;
	// /**
	//  * Called when Montage starts blending out and is not interrupted
	//  */
	// UPROPERTY(BlueprintAssignable)
	// FGameplayTaskMontagePlayDelegate OnBlendOut;
	/**
	 * 监听到执行操作
	 */
	UPROPERTY(BlueprintAssignable)
	FGameplayTaskMontagePlayDelegate OnNotifyBegin;
	/**
	 * 监听到事件结束
	 */
	UPROPERTY(BlueprintAssignable)
	FGameplayTaskMontagePlayDelegate OnNotifyEnd;
#pragma endregion

protected:
	/**
	 * 激活时执行
	 */
	virtual void Activate() override;
	/**
	 * 结束后执行
	 */
	virtual void OnDestroy(bool bInOwnerFinished) override;
public:
	/**
	 * 自定义蒙太奇管理，在IGameplayTaskOwnerInterface中使用
	 *
	 * @param TaskOwner 使用的对象
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param InSkeletalMeshComponent 需要播放蒙太奇的对象
	 * @param InMontageToPlay The montage to play on the character
	 * @param Rate 播放速率
	 * @param StartSection 从哪个Section开始播放
	 * @param bStopWhenAbilityEnds 在技能结束后是否结束播放
	 * @param StartTimeSeconds 从哪个时间点开始播放
	 */
	UFUNCTION(BlueprintCallable, Category="Gameplay Task", meta = (DisplayName="Gameplay Task Play Montage",
		HidePin = "TaskOwner", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UGameplayTask_PlayMontage* CreateGameplayTaskPlayMontageProxy(
		TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner,
		USkeletalMeshComponent* InSkeletalMeshComponent,
		FName TaskInstanceName,
		UAnimMontage* InMontageToPlay, float Rate = 1.f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float StartTimeSeconds = 0.f);
	/**
	 * 自定义蒙太奇管理，在Gameplay Ability中使用
	 *
	 * @param TaskOwner 使用的对象
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param InMontageToPlay The montage to play on the character
	 * @param Rate 播放速率
	 * @param StartSection 从哪个Section开始播放
	 * @param bStopWhenAbilityEnds 在技能结束后是否结束播放
	 * @param StartTimeSeconds 从哪个时间点开始播放
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="Task Play Montage",
		HidePin = "TaskOwner", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UGameplayTask_PlayMontage* CreateTaskPlayMontageProxy(
		class UGameModeAbility_Montage* TaskOwner,
		FName TaskInstanceName,
		UAnimMontage* InMontageToPlay,
		float Rate = 1.f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float StartTimeSeconds = 0.f);
	/**
	 * 自定义蒙太奇管理，在UGameMode Ability Single Montage中使用
	 *
	 * @param TaskOwner 使用的对象
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param Rate 播放速率
	 * @param StartSection 从哪个Section开始播放
	 * @param bStopWhenAbilityEnds 在技能结束后是否结束播放
	 * @param StartTimeSeconds 从哪个时间点开始播放
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="Task Play Single Montage",
		HidePin = "TaskOwner", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UGameplayTask_PlayMontage* CreateTaskPlaySingleMontageProxy(
		class UGameModeAbility_SingleMontage* TaskOwner,
		FName TaskInstanceName,
		float Rate = 1.f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float StartTimeSeconds = 0.f);

#pragma region 参数
protected:
	/**
	 * 正在播放的蒙太奇动画
	 */
	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;
	/**
	 * 需要播放蒙太奇动画的对象
	 */
	UPROPERTY()
	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	/**
	 * 播放速率
	 */
	UPROPERTY()
	float Rate;
	/**
	 * 从哪个Section开始播放
	 */
	UPROPERTY()
	FName StartSection;
	/**
	 * 从哪个时间点开始播放
	 */
	UPROPERTY()
	float StartTimeSeconds;
	/**
	 * 在技能结束后是否结束播放
	 */
	UPROPERTY()
	bool bStopWhenAbilityEnds;
protected:
	// /**
	//  * BlendingOut事件监听
	//  */
	// UFUNCTION()
	// void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	/**
	 * 蒙太奇结束事件监听
	 */
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	/**
	 * 动画实例
	 */
	TWeakObjectPtr<UAnimInstance> AnimInstancePtr;
	/**
	 * 蒙太奇播放后的实例ID
	 */
	int32 MontageInstanceID;
	// /**
	//  * 在BindingOut判断时是否是被打断的情况
	//  */
	//bool bInterruptedCalledBeforeBlendingOut = false;
	/**
	 * 绑定的事件
	 */
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	/**
	 * 绑定的事件
	 */
	FOnMontageEnded MontageEndedDelegate;
	/**
	 * 指令堆栈
	 */
	//UPROPERTY()
	TArray<TTuple<FName, FTransparentParameters*>> NotifyCache;

private:
	/**
	 * 移除绑定事件
	 */
	void UnbindDelegates();
	/**
	 * 监听到事件
	 */
	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	/**
	 * 监听到事件结束
	 */
	UFUNCTION()
	void OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
#pragma endregion
};
