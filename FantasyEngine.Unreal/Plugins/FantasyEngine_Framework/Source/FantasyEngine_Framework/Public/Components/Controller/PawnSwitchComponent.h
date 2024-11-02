// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnSwitchComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchToNewPawnDynamic, APawn*, Target);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSwitchToNewPawnNative, APawn*);

/** 角色切换类 */
UCLASS(Blueprintable, NotBlueprintType, ClassGroup="Fantasy Engine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UPawnSwitchComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPawnSwitchComponent();

#pragma region 角色切换函数

public:
	/**
	 * 切换控制器到创建的角色上
	 * @param InLocation 起始位置
	 * @param InRotator 起始方向
	 * @param bSmooth 是否平滑过渡
	 * @param Duration 过度时长
	 * @param BlendFunc 平滑效果
	 * @return 生成的角色类
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller")
	APawn* SwitchToPawnCreatedByClass(const FVector InLocation,
	                                  const FRotator InRotator,
	                                  const bool bSmooth = false,
	                                  const float Duration = 1.f,
	                                  const EViewTargetBlendFunction BlendFunc = VTBlend_EaseIn);

public:
	/** 平滑相机移动到新的角色上 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller")
	void SlideToPawn(APawn* Target, const float Duration = 1.f,
	                 const EViewTargetBlendFunction BlendFunc = VTBlend_EaseIn);

	/** 是否平滑切换Pawn */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller")
	void SwitchToPawn(APawn* Target);

private:
	/** 切换 Pawn 的实现 */
	void SwitchToPawnImpl(APawn* Target, const bool bSmooth, const float Duration,
	                      const EViewTargetBlendFunction BlendFunc);
#pragma endregion

public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Switch To New Pawn")
	FOnSwitchToNewPawnDynamic K2_OnSwitchToNewPawn;
	FOnSwitchToNewPawnNative OnSwitchToNewPawn;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION(BlueprintPure, Category="Controller")
	APlayerController* GetPlayerController() const;

#pragma region Variables

private:
	/** 相机关注的目标 */
	UPROPERTY()
	TWeakObjectPtr<APawn> SwitchTarget = nullptr;
	/** 过渡时间，负数表示无效 */
	UPROPERTY()
	float TotalSeconds = -1.f;

public:
	/** 用于创建的角色模板类 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Controller", DisplayName="Pawn")
	TSubclassOf<APawn> PawnClass = nullptr;

#pragma endregion
};
