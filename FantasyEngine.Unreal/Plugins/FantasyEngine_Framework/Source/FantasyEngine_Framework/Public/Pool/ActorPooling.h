// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pool/ObjectPooling.h"
#include "ActorPooling.generated.h"
/* 对象池类型 */
UENUM(BlueprintType)
enum class EActorPoolParentType : uint8
{
	/**
	 * 错误类型
	 */
	None,
	/**
	 * Actor
	 */
	Actor,
	/**
	 * 组件
	 */
	Component,
};

/**
 * Actor对象池
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UActorPooling final : public UObjectPooling
{
	GENERATED_BODY()
private:
	/**
	 * 对象池类型
	 */
	UPROPERTY()
	EActorPoolParentType ParentType;
	/**
	 * 父物体
	 */
	UPROPERTY()
	UObject* Parent;

public:
	/**
	 * 对象池类型
	 */
	UFUNCTION(BlueprintPure)
	EActorPoolParentType GetParentType() const;
	/**
	 * 设置父物体
	 */
	UFUNCTION(BlueprintCallable)
	void SetParentOfActor(AActor* Actor);
	/**
	 * 设置父物体
	 */
	UFUNCTION(BlueprintCallable)
	void SetParentOfComponent(USceneComponent* Component);
	/**
	 * 重置父物体
	 */
	UFUNCTION(BlueprintCallable)
	void ResetParent();
	/**
	 * 获取所有正在使用的Actor
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool",
		meta=(DeterminesOutputType="InActorClass", DynamicOutputParam="OutActors"))
	void GetUsingActors(TSubclassOf<AActor> InActorClass, TArray<AActor*>& OutActors) const;
private:
	/**
	 * 创建时的物体位置
	 */
	FVector InLocation = FVector::ZeroVector;
	/**
	 * 创建时的物体方向
	 */
	FRotator InRotator = FRotator::ZeroRotator;
	/**
	 * 创建时的物体大小
	 */
	FVector InScale = FVector::OneVector;
	/**
	 * 以上参数是否是局部单位
	 */
	bool bInRelative = false;

public:
	/**
	 * 获取可使用对象
	 * @param Location 物体位置
	 * @param Rotator 物体方向
	 * @param Scale 物体大小
	 * @param bRelative 是否是局部单位
	 * @return 可使用对象
	 */
	UFUNCTION(BlueprintCallable, DisplayName="Get Actor", meta=(DeterminesOutputType="Class"))
	UPoolItemHandle* GetActor(FVector Location = FVector(0.f, 0.f, 0.f),
	                          FRotator Rotator = FRotator(0.f, 0.f, 0.f),
	                          FVector Scale = FVector(1.f, 1.f, 1.f), bool bRelative = false);

protected:
	/**
	 * 重写创造函数
	 */
	virtual UObject* CreateObject() override;
	/**
	 * 重写使用处理
	 */
	virtual void BeforeUse(UPoolItemHandle* Handle) override;
	/**
	 * 重写使用处理
	 */
	virtual void OnUse(UPoolItemHandle* Handle) override;
	/**
	 * 重写回收处理
	 */
	virtual void OnCollect(UPoolItemHandle* Handle) override;
};
