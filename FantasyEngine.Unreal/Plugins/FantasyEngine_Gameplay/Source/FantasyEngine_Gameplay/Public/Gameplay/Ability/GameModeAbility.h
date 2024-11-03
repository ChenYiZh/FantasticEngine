// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTaskOwnerInterface.h"
#include "Defines/AbilityStopCode.h"
#include "Gameplay/AttackContext.h"
#include "GameModeAbility.generated.h"

/**
 * 技能类
 * 需要手动调用Finish函数
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASYENGINE_GAMEPLAY_API UGameModeAbility : public UObject, public IGameplayTaskOwnerInterface
{
	GENERATED_BODY()
	friend class UGameplayCharacterComponent;
	friend class UGameEffect;
#pragma region 技能任务管理
	/** List of currently active tasks, do not modify directly */
	UPROPERTY()
	TArray<TObjectPtr<UGameplayTask>> ActiveTasks;
#pragma endregion

#pragma region 配置信息
private:
	/**
	 * 是否操作完成
	 */
	bool bCompleted = false;

public:
	/**
	 * 是否操作完成
	 */
	UFUNCTION(BlueprintPure, Category="Ability|Condition")
	bool IsCompleted() const;
private:
	/**
	 * 执行时间
	 */
	double BeginActiveTime;
	/**
	 * 默认积压在堆栈中的数量，大于0时有效
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Configs")
	int32 MaxCountInStack = 0;
	/**
	 * 超出堆栈限制时的策略，True 使用最新的，将最早的删除，False 维持现状
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Configs", meta=(EditCondition="MaxCountInStack>0"))
	bool bInterruptOldAndApplyNew = true;
public:
	/**
	 * 持续时间（ <0 表示不使用持续时间）
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability|Configs")
	float Duration = -1;
#pragma endregion

#pragma region 基本属性
private:
	/**
	 * 角色对象
	 */
	UPROPERTY(VisibleAnywhere, Category="Ability")
	class UGameplayCharacterComponent* Character;
	// /**
	//  * 决策管理类
	//  */
	// UPROPERTY(VisibleAnywhere, Category="Ability")
	// class UActorStrategy* Strategy;
public:
	/**
	 * 获取角色对象
	 */
	UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="Ability", DisplayName="Ori Character")
	UGameplayCharacterComponent* GetCharacter(TSubclassOf<UGameplayCharacterComponent> Class = nullptr) const;
	/**
	 * 获取角色对象
	 */
	template <typename T>
	T* GetCharacter() { return Cast<T>(Character); }

	// /**
	//  * 获取决策管理类
	//  */
	// UFUNCTION(BlueprintPure, meta=(DeterminesOutputType="Class"), Category="Ability", DisplayName="Ori Strategy")
	// class UActorStrategy* GetOriStrategy(TSubclassOf<UActorStrategy> Class) const;
	// /**
	//  * 获取决策管理类
	//  */
	// template <typename T>
	// T* GetOriStrategy() { return Cast<T>(GetOriStrategy(nullptr)); }
#pragma endregion

#pragma region 标签管理
protected:
	/**
	 * 技能标识
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Tags")
	FGameplayTagContainer AbilityTags;
	/**
	 * 只有角色没有包含这些标签时才能执行
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Tags")
	FGameplayTagContainer ActiveIfExclude;
	/**
	 * 只有角色包含这些标签时才执行
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Tags")
	FGameplayTagContainer ActiveIfExists;
	/**
	 * 在技能发动阶段阻止并打断哪些标签的技能
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Tags")
	FGameplayTagContainer BlockIfExecuting;
	/**
	 * 当这些Tags激活时，当前的技能会被自动取消
	 */
	UPROPERTY(EditAnywhere, Category="Ability|Tags")
	FGameplayTagContainer InterruptByTags;
#pragma endregion

#pragma region 生命周期管理
private:
	/**
	 * 是否已经开始执行
	 */
	bool bActived = false;
public:
	/**
	 * 是否已经开始执行
	 */
	UFUNCTION(BlueprintPure, Category="Ability|Condition")
	bool IsActived() const;
	/**
	 * 开始执行
	 */
	void Active();
protected:
	/**
	 * 是否可以执行判断
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Ability|Condition")
	bool CanActive();
	/**
	 * 预处理
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Events", DisplayName="PreActive")
	void K2_PreActive();
	/**
	 * 预处理
	 */
	virtual void PreActive();
	/**
	 * 开始执行
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Events", DisplayName="On Active")
	void K2_OnActive();
	/**
	 * 开始执行
	 */
	virtual void OnActive();
	/**
	 * 结束事件
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Events", DisplayName="In The End")
	void K2_InTheEnd(const EAbilityStopCode Reason);
	/**
	 * 结束事件
	 */
	virtual void InTheEnd(const EAbilityStopCode Reason);
public:
	/**
	 * 退出事件，基本用于外部调用
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Events")
	void Stop(const EAbilityStopCode Reason = EAbilityStopCode::Interrupt);

protected:
	/**
	 * 帧事件
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Ability|Events")
	void Tick(float DeltaSeconds);
	/**
	 * 帧事件
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Events")
	void TickAbililty(float DeltaSeconds);

protected:
	/**
	 * 整个Ability执行完成时调用
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Events")
	void Finish();
#pragma endregion

#pragma region 碰撞检测
protected:
	/**
	 * 通过Class检测碰撞
	 */
	UFUNCTION(BlueprintPure, Category="Ability|Overlap")
	void GetOverlappingActorsOfClass(TArray<AActor*>& OverlapingActors, TSubclassOf<AActor> ActorClass,
	                                 const AActor* Source = nullptr) const;
	/**
	 * 通过Actor的标签进行检测
	 */
	UFUNCTION(BlueprintPure, Category="Ability|Overlap")
	void GetOverlappingActorsOfTag(TArray<AActor*>& OverlapingActors, const FName& InTag,
	                               const AActor* Source = nullptr) const;
	/**
	 * 通过Actor的标签进行检测
	 */
	UFUNCTION(BlueprintPure, Category="Ability|Overlap")
	void GetOverlappingActorsOfTags(TArray<AActor*>& OverlapingActors, const TArray<FName>& Tags,
	                                const AActor* Source = nullptr) const;

	/**
	 * 从Actor队列中找到距离最近的Actor
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Overlap")
	AActor* FindNearestActor(const TArray<AActor*>& Actors) const;
	/**
	 * 从Actor队列中找到距离最近的Actor，并且看向他
	 * @param Actors 查询的队列
	 * @param bFocus 是否调用注视
	 * @param bKeep 是否一直保持指向
	 * @param bInUseTurn 是否使用转身过渡
	 * @param bLookAtOnEmpty 当没有传入队列为空时，是否继续执行LookAt，这样会执行角色的取消注视
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Overlap")
	AActor* FindNearestActorAndLookAt(const TArray<AActor*>& Actors, const bool bFocus = true, const bool bKeep = false,
	                                  const bool bInUseTurn = false, const bool bLookAtOnEmpty = true);
	/**
	 * 使用这个技能角色看向指定目标
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Overlap")
	void LookAtActor(AActor* Target, const bool bFocus = true, const bool bKeep = false,
	                 const bool bInUseTurn = false);
#pragma endregion

#pragma region 攻击检测
public:
	/**
	 * 只创建攻击计算类
	 */
	UFUNCTION(BlueprintPure, Category="Ability|Attack")
	class UGameEffect* MakeGameEffectOnly(TSubclassOf<UGameEffect> Class, AActor* Target) const;

	/**
	 * 创建并执行攻击计算
	 * @param Class 攻击计算类
	 * @param Target 目标对象
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Attack")
	void MakeAndApplyGameEffect(TSubclassOf<UGameEffect> Class,
	                            AActor* Target,
	                            const int32 TargetIndex = 0,
	                            FTransparentParameters Parameters = FTransparentParameters());

protected:
	/**
	 * 攻击检测时会把检测到的对象放入这个缓存池
	 */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Ability|Attack")
	TArray<AActor*> OverlappingActors;
protected:
	/**
	 * 目标对象是否可以作为攻击对象
	 * @param Class 攻击计算类
	 * @param Target 目标对象
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param Parameters 透传参数
	 * @return 返回True将执行攻击，反之则取消攻击
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Ability|Attack")
	bool CanAttack(/*UPARAM(ref)*/ TSubclassOf<UGameEffect>& Class,
	                               /*UPARAM(ref)*/ AActor* Target,
	                               const int32& TargetIndex,
	                               UPARAM(ref) FTransparentParameters& Parameters);
	/**
	 * 目标对象是否可以作为攻击对象
	 * @param Target 目标对象
	 * @param Context 攻击上下文
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Ability|Attack")
	void OnAttackActor(AActor* Target, const FAttackContext& Context);
protected:
	/**
	 * 碰撞检测后进行攻击
	 * @param GameEffect 攻击计算类
	 * @param bClearCache 是否清楚检测列表缓存
	 * @return 是否触发了攻击
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Attack")
	bool AttackOverlappingActors(TSubclassOf<UGameEffect> GameEffect,
	                             FTransparentParameters Parameters, const bool bClearCache = true);
	/**
	 * 碰撞检测后进行攻击
	 * @param ActorClass 进行判断的Actor类
	 * @param GameEffect 攻击计算类
	 * @param bClearCache 是否清楚检测列表缓存
	 * @return 是否触发了攻击
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Attack")
	bool AttackOverlappingActorsOfClass(TSubclassOf<AActor> ActorClass, TSubclassOf<UGameEffect> GameEffect,
	                                    FTransparentParameters Parameters, const bool bClearCache = true);
	/**
	 * 碰撞检测后进行攻击
	 * @param InTag 进行判断的Actor类的标签
	 * @param GameEffect 攻击计算类
	 * @param bClearCache 是否清楚检测列表缓存
	 * @return 是否触发了攻击
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Attack")
	bool AttackOverlappingActorsOfTag(const FName InTag, TSubclassOf<UGameEffect> GameEffect,
	                                  FTransparentParameters Parameters, const bool bClearCache = true);
	/**
	 * 碰撞检测后进行攻击
	 * @param Tags 进行判断的Actor类的标签
	 * @param GameEffect 攻击计算类
	 * @param bClearCache 是否清楚检测列表缓存
	 * @return 是否触发了攻击
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Attack")
	bool AttackOverlappingActorsOfTags(const TArray<FName>& Tags, TSubclassOf<UGameEffect> GameEffect,
	                                   FTransparentParameters Parameters, const bool bClearCache = true);

private:
	/**
	 * 碰撞检测后进行攻击，使用Lambda进行获取列表
	 * @param GameEffect 攻击计算类
	 * @param bClearCache 是否清楚检测列表缓存
	 * @param Predicate Lambda
	 * @return 是否触发了攻击
	 */
	template <class PREDICATE_CLASS>
	bool AttackOverlappingActors(TSubclassOf<UGameEffect> GameEffect,
	                             FTransparentParameters& Parameters,
	                             const bool& bClearCache,
	                             const PREDICATE_CLASS& Predicate)
	{
		bool bOverlap = false;
		if (!IsValid(GameEffect)) { return false; }
		if (bClearCache) { OverlappingActors.Empty(); }
		TArray<AActor*> InOverlappingActors = Invoke(Predicate);
		int32 Index = 0;

		int32 Identity = INDEX_NONE;
		double Number = 0.f;
		FName FTag = NAME_None;
		FString Content;

		for (AActor* Target : InOverlappingActors)
		{
			if (!OverlappingActors.Contains(Target))
			{
				if (CanAttack(GameEffect, Target, Index, Parameters))
				{
					MakeAndApplyGameEffect(GameEffect, Target, Index, Parameters);
					bOverlap = true;
					Index++;
				}
			}
		}

		return bOverlap;
	}
#pragma endregion

#pragma region IGameplayTaskOwnerInterface
	// --------------------------------------
	//	IGameplayTaskOwnerInterface
	// --------------------------------------	
	virtual UGameplayTasksComponent* GetGameplayTasksComponent(const UGameplayTask& Task) const override;
	virtual AActor* GetGameplayTaskOwner(const UGameplayTask* Task) const override;
	virtual AActor* GetGameplayTaskAvatar(const UGameplayTask* Task) const override;
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
#pragma endregion
};
