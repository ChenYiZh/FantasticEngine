// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TweenComponent.generated.h"

/** Tween 动画类型 */
UENUM(BlueprintType)
enum class ETweenPlayStyle:uint8
{
	/** 只执行一次 */
	Once = 0,
	/** 循环播放 */
	Loop = 1,
	/** 反复播放 */
	PingPong = 2
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTweenPlayDynamic, USceneComponent*, Component, float, Alpha);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTweenPlayNative, USceneComponent*, float);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTweenFinishedDynamic);

DECLARE_MULTICAST_DELEGATE(FOnTweenFinishedNative);

/** Tween 动画基类，如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType, Abstract,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UTweenComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// 启用时间控件
	UTweenComponent();

protected:
	// 开始时获取一下可以操作的控件
	virtual void BeginPlay() override;

public:
	// 帧事件
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Functions

public:
	/** Alpha = [0, 1] 根据传入参数显示对应状态 */
	virtual void Play(float Alpha);
	/** 在做表现层时调用的函数 */
	UPROPERTY(BlueprintAssignable, DisplayName="On Play")
	FOnTweenPlayDynamic K2_OnPlay;
	/** 在做表现层时调用的函数 */
	FOnTweenPlayNative OnPlay;

protected:
	/** 在做表现层时调用的函数 */
	UFUNCTION(BlueprintNativeEvent, Category="Tween")
	void Play(USceneComponent* Component, float Alpha);

public:
	/** 正向播放 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void PlayForward();
	/** 反向播放 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void PlayBack();
	/** 重置到正向起始状态 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void ResetToBegin();
	/** 重置到反向起始状态 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void ResetToEnd();
	/** 暂停 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void Pause();
	/** 播放 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void Resume();
	/** 停止 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void Stop();
	/** 寻找可以操作的控件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void FindComponent();

protected:
	/** 一次播放结束时执行的事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tween")
	void OnceFinished();
	/** 当物体状态检测执行 */
	UFUNCTION(BlueprintNativeEvent, Category="Tween")
	void CheckComponentState();
	/** 当物体暂停或隐藏时执行 */
	UFUNCTION(BlueprintNativeEvent, Category="Tween")
	void OnEnable();
	/** 当物体显示或启动时执行 */
	UFUNCTION(BlueprintNativeEvent, Category="Tween")
	void OnDisable();
#pragma endregion


#pragma region Variables

public:
	/** 是否自动启动 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween|Tweener")
	bool bPlayOnEnable = false;
	/** 是否自动触发 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween|Tweener")
	ETweenPlayStyle PlayStyle = ETweenPlayStyle::Once;
	/** 播放速率曲线，只使用到 X = [0, 1] 的区间值。如果空就播放线性动画。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween|Tweener")
	TObjectPtr<UCurveFloat> FloatCurve = nullptr;
	/** 延迟多少秒播放 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween|Tweener")
	float StartDelay = 0;
	/** 播放时长 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween|Tweener")
	float Duration = 1;
	/** 是否在隐藏时调整到初始状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween", AdvancedDisplay)
	bool bResetOnDisable = false;
	/** 一次时长播放多少次，Time <= 1 只播放一次 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween", AdvancedDisplay)
	int Time = 1;
	/** 每次播放间隔 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween", AdvancedDisplay)
	float Interval = 0;
	/** 播放速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween", AdvancedDisplay)
	float Speed = 1.0f;

public:
	/** 每轮播放结束时都会调用 */
	UPROPERTY(BlueprintAssignable, DisplayName="On Finished")
	FOnTweenFinishedDynamic K2_OnFinished;
	/** 每轮播放结束时都会调用 */
	FOnTweenFinishedNative OnFinished;

protected:
	/** 调用OnFinished函数 */
	void CallOnFinished() const
	{
		OnFinished.Broadcast();
		K2_OnFinished.Broadcast();
	}
#pragma endregion

private:
	/** 是否还在播放动画 */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Tween|Tweener")
	bool bIsPlaying = false;

	/** 是否是回放 */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Tween|Tweener")
	bool bPlayingBack = false;

	/** 正在控制的控件 */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Tween|Tweener", DisplayName="Component")
	TObjectPtr<USceneComponent> ControlledComponent = nullptr;

public:
	/** 是否还在播放动画 */
	FORCEINLINE bool IsPlaying() const { return bIsPlaying; }
	/** 是否是回放 */
	FORCEINLINE bool PlayingBack() const { return bPlayingBack; }
	/** 正在控制的控件 */
	FORCEINLINE USceneComponent* GetComponent() const { return ControlledComponent; }


#pragma region 局部变量

private:
	/** 已经在动画启动时等待的时间 */
	UPROPERTY()
	float RemainingUntilStart = 0;
	/** 排除启动等待的时间，动画剩余的时间 */
	UPROPERTY()
	float Remaining = -1;
	/** 操作组件在上一帧是否是可用状态 */
	UPROPERTY()
	bool bLastEnabled = false;
	/** 多次播放中的一次所需要的时长 */
	UPROPERTY()
	float SingleDuration = 0;
	/** 判断有没有初始化过 */
	UPROPERTY()
	bool bInitialized = false;
#pragma endregion
};
