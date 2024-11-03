// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ActorHitter.h"
#include "Gameplay/Attribute/CharacterAttributeSet.h"
#include "Gameplay/Ability/GameModeAbility.h"
#include "GameplayTagContainer.h"
#include "EquipmentComponent.h"
#include "Gameplay/Attribute/AttributeCollection.h"
#include "Defines/DieCode.h"
#include "GameFramework/Character.h"
#include "Components/Pawn/CharacterBeingComponent.h"
#include "GameplayCharacterComponent.generated.h"

/** 包含技能生命周期操作以及生命周期的 Character 组件。注意：Actor需要包含 GameplayTasksComponent 组件 */
UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_GAMEPLAY_API UGameplayCharacterComponent : public UCharacterBeingComponent, public IActorHitter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	UGameplayCharacterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma region 默认组件

private:
	/**
	 * 技能任务管理
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FCharacter|Camera", meta = (AllowPrivateAccess = "true"))
	class UGameplayTasksComponent* GameplayTasks;

public:
	/** 技能任务管理 **/
	FORCEINLINE class UGameplayTasksComponent* GetGameplayTasks() const { return GameplayTasks; }

#pragma endregion


#pragma region 标签管理

private:
	/**
	 * Character上存在的标签
	 */
	UPROPERTY(VisibleAnywhere, Category="FCharacter|Attributes")
	FGameplayTagContainer GameplayTags;

public:
	/**
	 * 获取标签管理
	 */
	UFUNCTION(BlueprintPure, Category="FCharacter|Attributes", DisplayName="Tags")
	void GetTags(FGameplayTagContainer& Container) const;
	/**
	 * 是否包含标签
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Attributes")
	bool HasTag(const FGameplayTag Tag);
	/**
	 * 添加标签
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Attributes")
	void AddTag(const FGameplayTag Tag);
	/**
	 * 移除标签
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Attributes")
	void RemoveTag(const FGameplayTag Tag);
	/**
	 * 移除所有标签
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Attributes")
	void ClearTags();
#pragma endregion

#pragma region 属性管理

private:
	/**
	 * 属性类的基类
	 */
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess, HideViewOptions, AllowAbstract="true"),
		Category="FCharacter|Attributes", DisplayName="Default Attributes")
	TSubclassOf<UCharacterAttributeSet> AttributesClass = UCharacterAttributeSet::StaticClass();
	/**
	 * 属性类
	 */
	UPROPERTY(VisibleInstanceOnly, Category="FCharacter|Attributes")
	UCharacterAttributeSet* Attributes = nullptr;

	/**
	 * 属性集合
	 */
	UPROPERTY(VisibleInstanceOnly, Category="FCharacter|Attributes")
	UAttributeCollection* AttributeCollection;

public:
	/**
	 * 在设置属性时调用
	 * @param InAttributes 属性
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="FCharacter|Attributes", DisplayName="On Setting Attributes")
	void K2_SetAttributes(UCharacterAttributeSet* InAttributes);
	/**
	 * 在设置属性时调用
	 * @param InAttributes 属性
	 */
	UFUNCTION(BlueprintNativeEvent, Category="FCharacter|Attributes")
	void SetAttributes(UCharacterAttributeSet* InAttributes);

	/**
	 * 获取属性类
	 */
	UFUNCTION(BlueprintPure, Exec, Category="FCharacter|Attributes", DisplayName="Attributes",
		meta=(DeterminesOutputType="Class"))
	UCharacterAttributeSet* GetCharacterAttributes(TSubclassOf<UCharacterAttributeSet> Class = nullptr) const;

	/**
	 * 获取属性类
	 */
	template <typename T>
	T* GetAttributes() const
	{
		return Cast<T>(Attributes);
	}

	UFUNCTION(BlueprintPure, Exec, Category="FCharacter|Attributes")
	UAttributeCollection* GetAttributeCollection() const;
#pragma endregion

#pragma region 武器

private:
	UPROPERTY(VisibleAnywhere, Category="FCharacter|Equipment")
	TArray<UEquipmentComponent*> Equipments;

public:
	UFUNCTION(BlueprintPure, Exec, Category="FCharacter|Attributes")
	const TArray<UEquipmentComponent*>& GetEquipments() const;

	UFUNCTION(BlueprintCallable, Category="FCharacter|Equipment")
	void PutOnEquipment(UEquipmentComponent* Equipment, FName SocketName, EAttachmentRule LocationRule,
	                    EAttachmentRule RotationRule,
	                    EAttachmentRule ScaleRule, bool bWeldSimulatedBodies);

	UFUNCTION(BlueprintCallable, Category="FCharacter|Equipment")
	void PutOffEquipment(UEquipmentComponent* Equipment, EDetachmentRule LocationRule = EDetachmentRule::KeepRelative,
	                     EDetachmentRule RotationRule = EDetachmentRule::KeepRelative,
	                     EDetachmentRule ScaleRule = EDetachmentRule::KeepRelative);
#pragma endregion

#pragma region 正在触发的技能列表

private:
	/**
	 * 正在触发的技能列表
	 */
	UPROPERTY(VisibleInstanceOnly, Category="FCharacter|Abilities")
	TArray<UGameModeAbility*> Abilities;

public:
	/**
	 * 正在触发的技能列表
	 */
	UFUNCTION(BlueprintPure, Category="FCharacter|Abilities", DisplayName="Abilities")
	const TArray<UGameModeAbility*> GetAbilities();

	/**
	 * 根据Class创建技能，但不会执行
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	UGameModeAbility* MakeAbility(TSubclassOf<UGameModeAbility> AbilityClass);

	/**
	 * 根据Class创建技能，并且执行，如果判断无效则返回为空
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	UGameModeAbility* MakeAndActiveAbility(TSubclassOf<UGameModeAbility> AbilityClass);

	/**
	 * 执行技能操作
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	bool ActiveAbility(UGameModeAbility* Ability);

	/**
	 * 是否包含某一个技能
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	bool ContainsAbility(UGameModeAbility* Ability);

	/**
	 * 通过Class判断是否包含某个技能
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	bool ContainsAbilityOfClass(TSubclassOf<UGameModeAbility> AbilityClass);

	/**
	 * 移除某个技能，会执行打断操作
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	bool RemoveAbility(UGameModeAbility* Ability);

	/**
	 * 通过Class移除所有指定技能，会执行打断操作
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	bool RemoveAbilityOfClass(TSubclassOf<UGameModeAbility> AbilityClass);

	/**
	 * 通过Tag移除所有指定技能，会执行打断操作
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	bool RemoveAbilityOfTags(const FGameplayTagContainer RemovableTags);

	/**
	 * 通过Tag移除所有指定技能，会执行打断操作
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	bool RemoveAbilityOfTag(const FGameplayTag RemovableTag);

	/**
	 * 移除所有技能，会执行打断操作
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Abilities")
	void RemoveAllAbilities();

private:
	/**
	 * 移除所有技能，并且清空队列
	 */
	void RemoveAndClearAllAbilities();
#pragma endregion

#pragma region 策略组
	//
	// private:
	// 	/**
	// 	 * 策略组基类
	// 	 */
	// 	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess, HideViewOptions, AllowAbstract="true"),
	// 		Category="FCharacter|Strategy", DisplayName="Default Strategy")
	// 	TSubclassOf<UActorStrategy> StrategyClass = UActorStrategy::StaticClass();
	// 	/**
	// 	 * 策略组
	// 	 */
	// 	UPROPERTY(VisibleInstanceOnly, Category="FCharacter|Strategy")
	// 	UActorStrategy* Strategy = nullptr;
	//
	// public:
	// 	/**
	// 	 * 在设置属性时调用
	// 	 * @param InStrategy 属性
	// 	 */
	// 	UFUNCTION(BlueprintImplementableEvent, Category="FCharacter|Strategy", DisplayName="On Setting Strategy")
	// 	void K2_SetStrategy(UActorStrategy* InStrategy);
	// 	/**
	// 	 * 在设置属性时调用
	// 	 * @param InStrategy 策略组
	// 	 */
	// 	UFUNCTION(BlueprintNativeEvent, Category="FCharacter|Strategy")
	// 	void SetStrategy(TSubclassOf<UActorStrategy> InStrategy = nullptr);
	// 	/**
	// 	 * 获取策略组
	// 	 */
	// 	UFUNCTION(BlueprintPure, Category="FCharacter|Strategy", DisplayName="Ori Strategy",
	// 		meta=(DeterminesOutputType="Class"))
	// 	UActorStrategy* GetOriStrategy(TSubclassOf<UActorStrategy> Class = nullptr) const;
	// 	/**
	// 	 * 获取策略组
	// 	 */
	// 	template <typename T>
	// 	T* GetOriStrategy() const
	// 	{
	// 		return Cast<T>(Strategy);
	// 	}
#pragma endregion

#pragma region 战斗生命周期

public:
	/**
	 * 执行攻击
	 * @param GameEffect 攻击的计算公式
	 * @param Target 被攻击对象
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param SourceAbility 从哪个技能发动的攻击
	 * @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Battle")
	void Attack(UGameEffect* GameEffect,
	            AActor* Target,
	            const int32 TargetIndex = 0,
	            UGameModeAbility* SourceAbility = nullptr,
	            FTransparentParameters Parameters = FTransparentParameters());
	/**
	 * 执行攻击
	 * @param GameEffect 攻击的计算公式
	 * @param Target 被攻击对象
	 * @param TargetIndex 攻击对象是这次攻击队列中的第几个
	 * @param SourceAbility 从哪个技能发动的攻击
	* @param Parameters 透传参数
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Battle")
	void AttackOfClass(TSubclassOf<UGameEffect> GameEffect,
	                   AActor* Target,
	                   const int32 TargetIndex = 0,
	                   UGameModeAbility* SourceAbility = nullptr,
	                   FTransparentParameters Parameters = FTransparentParameters());
	/**
	 * 攻击
	 * @param Target 被攻击对象
	 * @param Context 攻击的上下文
	 */
	UFUNCTION(BlueprintCallable, Category="FCharacter|Battle")
	void AttackOfContext(AActor* Target, FAttackContext Context);

protected:
	/**
	 * 在攻击到目标时调用
	 * @param Target 目标
	 * @param Context 攻击的上下文
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="FCharacter|Battle")
	void OnAttackActor(AActor* Target, const FAttackContext& Context);
	/**
	 * 受击时调用
	 * @param Context 攻击的上下文
	 */
	virtual void Hit_Implementation(const FAttackContext& Context) override;

protected:
	/**
	 * 受击时调用
	 */
	UFUNCTION(Exec, BlueprintNativeEvent, Category="FCharacter")
	void OnHit(const FAttackContext& Context);
#pragma endregion

#pragma region 操作管理

public:
	/**
	 * 左上键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowUpPressed();
	/**
	 * 左上键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowUpReleased();
	/**
	 * 左下键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowDownPressed();
	/**
	 * 左下键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowDownReleased();
	/**
	 * 左左键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowLeftPressed();
	/**
	 * 左左键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowLeftReleased();
	/**
	 * 左右键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowRightPressed();
	/**
	 * 左右键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnArrowRightReleased();

	/**
	 * 右上键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionUpPressed();
	/**
	 * 右上键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionUpReleased();
	/**
	 * 右下键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionDownPressed();
	/**
	 * 右下键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionDownReleased();
	/**
	 * 右左键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionLeftPressed();
	/**
	 * 右左键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionLeftReleased();
	/**
	 * 右右键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionRightPressed();
	/**
	 * 右右键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FCharacter|Actions")
	void OnActionRightReleased();
#pragma endregion

public:
	/**
	 * 出生(重新使用)时，进行初始化操作
	 */
	virtual void Born() override;
	/**
	 * 死亡操作
	 */
	virtual void GoDie(const EDieCode Code) override;
	/**
	 * 对象销毁时执行
	 */
	virtual void BeginDestroy() override;
	/**
	 * 重置操作，Framework使用，非UE
	 */
	virtual void Reset() override;

#pragma region Variables

private:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category=Character, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

public:
	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }
#pragma endregion
};
