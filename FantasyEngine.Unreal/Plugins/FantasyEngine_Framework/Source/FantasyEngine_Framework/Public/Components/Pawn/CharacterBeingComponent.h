// Fill out your copyright notice in the Description page of Project Settings.
/*******************************************

ChenYiZh 源代码：
Framework Character 最基础的Character

修改 ChenYiZh 2023/4/18:

需求：
横板角色基类，使用3D角色为基础模板。
锁定3D角色的一些视角操作。

注意：
3D角色默认的视角是正对X轴，相机在视角后方
2D角色仍然需要角色正对Y轴，应该在角色上方再包裹一个对象，将相机与完整角色分离开来

修改 ChenYiZh 2023/5/9:
	移除F2DCharacter将2DCharacter的属性全部移动到FCharacter
修改 ChenYiZh 2024/4/16:
	将其改为Component使用
*******************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/Pawn/PawnOperationsComponent.h"
#include "Defines/DieCode.h"
#include "Pool/ObjectPooling.h"
#include "CharacterBeingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterBeingEventDynamic);

DECLARE_MULTICAST_DELEGATE(FCharacterBeingEventNative);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadDynamic, EDieCode, Code);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeadNative, EDieCode);

/** 拥有对象池并且包含出生死亡的标准 Character 组件 */
UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UCharacterBeingComponent : public UActorComponent, public IPoolItem
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterBeingComponent();

#pragma region 默认参数

private:
	/** 是否是还存活，GoDie和 Born会改变这个值 */
	UPROPERTY(VisibleAnywhere, Category="FCharacter|Attributes", meta=(DisplayPriority=-1))
	bool bAlive = false;

public:
	/** 是否是还存活，GoDie和Born会改变这个值 */
	UFUNCTION(BlueprintPure, Category="FCharacter|Attributes")
	bool IsAlive() const;
#pragma endregion


#pragma region 默认生命周期

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
#pragma endregion

#pragma region 生命周期

public:
	/**
	 * 出生(重新使用)时，进行初始化操作
	 */
	UFUNCTION(BlueprintCallable, Category="Character", DisplayName="Born")
	void K2_Born();
	/**
	 * 出生(重新使用)时，进行初始化操作
	 */
	virtual void Born();
	/**
	 * 死亡操作
	 */
	UFUNCTION(BlueprintCallable, Category="Character", DisplayName="Go Die")
	void K2_GoDie(const EDieCode Code = EDieCode::None);
	/**
	 * 死亡操作
	 */
	virtual void GoDie(const EDieCode Code = EDieCode::None);
	/**
	 * 重置操作，Framework使用，非UE
	 */
	UFUNCTION(BlueprintCallable, Category="Character", DisplayName="Resetf")
	void K2_Reset();
	/**
	 * 重置操作，Framework使用，非UE。
	 * 如果是对象池中的对象，则会自动回收到对象池中
	 */
	virtual void Reset();

protected:
	/**
	 * 出生(重新使用)时，进行初始化操作
	 * 在对象池中不需要使用这个函数
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character")
	void OnBorn();
	/**
	 * 死亡操作
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void OnGoDie(const EDieCode Code = EDieCode::None);
	/**
	 * 死亡操作
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void OnReset();

public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Born")
	FCharacterBeingEventDynamic K2_OnBorn;
	FCharacterBeingEventNative OnBornNative;

	UPROPERTY(BlueprintAssignable, DisplayName="On Dead")
	FOnDeadDynamic K2_OnDead;
	FOnDeadNative OnDead;

	UPROPERTY(BlueprintAssignable, DisplayName="On Reset")
	FCharacterBeingEventDynamic K2_OnReset;
	FCharacterBeingEventNative OnResetNative;
#pragma endregion

#pragma region 对象池生命周期

private:
	/**
	 * 对象池管理类
	 */
	UPROPERTY()
	UPoolItemHandle* PoolItemHandle;

public:
	/**
	 * 对象池管理类
	 */
	UFUNCTION(BlueprintPure, Category="Actor|Pool", DisplayName="Pool Item Handle")
	UPoolItemHandle* GetPoolItemHandle() const;
	/**
	 * 对象池中刚产生这个物体，初始化执行
	 */
	virtual void OnInitialize_Implementation(UPoolItemHandle* Handle) override;
	/**
	 * 开始使用这个物体时执行
	 */
	virtual void BeginToUse_Implementation() override;
#pragma endregion
};
