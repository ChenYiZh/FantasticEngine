// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObject.h"
#include "UObject/NoExportTypes.h"
#include "ObjectPooling.generated.h"

UINTERFACE(BlueprintType, MinimalAPI)
class UPoolItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 对象池子物体
 */
class FANTASYENGINE_FRAMEWORK_API IPoolItem
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * 对象池中刚产生这个物体，初始化执行
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Object Pool")
	void OnInitialize(class UPoolItemHandle* Handle);
	/**
	 * 开始使用这个物体时执行
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Object Pool")
	void BeginToUse();
	/**
	 * 释放这个物体，并且交回对象池时执行
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Object Pool")
	void OnRelease();
};

/**
 * 对象池子物体管理类
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UPoolItemHandle : public UWorldObject
{
	GENERATED_BODY()
	friend class UObjectPooling;
private:
	/**
	 * 使用的对象池
	 */
	UPROPERTY()
	UObjectPooling* Pooling = nullptr;
	/**
	 * 生成的对象
	 */
	UPROPERTY()
	UObject* Object = nullptr;

public:
	/**
	 * 正在使用的对象池
	 */
	UFUNCTION(BlueprintPure, Category="Object Pool", DisplayName="Pool", meta=(DeterminesOutputType="Class"))
	UObjectPooling* GetPooling(TSubclassOf<UObjectPooling> Class = nullptr) const;
	/**
	 * 释放这个物体，并且交回对象池
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool")
	void Release();
	/**
	 * 获取产生的对象
	 */
	UFUNCTION(BlueprintPure, Category="Object Pool", meta=(DeterminesOutputType="Class"))
	UObject* GetObject(TSubclassOf<UObject> Class = nullptr) const;
	/**
	 * 获取产生的对象
	 */
	template <class T>
	T* GetObject()
	{
		return Cast<T>(Object);
	}
};

/**
 * 对象池
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UObjectPooling : public UWorldObject
{
	GENERATED_BODY()
	friend class UPoolItemHandle;
protected:
	/**
	 * 对象父类
	 */
	UPROPERTY()
	TSubclassOf<UObject> ObjectClass;
protected:
	/**
	 * 正在使用的对象集合
	 */
	UPROPERTY()
	TArray<UPoolItemHandle*> UsingItems;
	/**
	 * 回收的对象集合
	 */
	UPROPERTY()
	TArray<UPoolItemHandle*> Collection;

public:
	/**
	 * 对象池总数，包括使用以及未使用的对象
	 */
	UFUNCTION(BlueprintPure, Category="Object Pool")
	int GetTotalCount() const;

protected:
	/**
	 * 产生了对象，准备开始使用
	 */
	virtual void BeforeUse(UPoolItemHandle* Handle);

public:
	/**
	 * 初始化函数，WidgetPool使用InitializeWidgetPool
	 * @param InObjectClass 
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool")
	void Initialize(TSubclassOf<UObject> InObjectClass);
	/**
	 * 获取对象池中可用的一个元素
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool", DisplayName="Get Item Handle", meta=(DeterminesOutputType="Class"))
	UPoolItemHandle* GetItemHandle();
	/**
	 * 释放所有正在用的元素
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool")
	void Release();

	/**
	 * 清空对象池
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool")
	virtual void Reset();
	/**
	 * 获取所有正在使用的Object
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool",
		meta=(DeterminesOutputType="InObjectClass", DynamicOutputParam="OutObjects"))
	void GetUsingItems(TSubclassOf<UObject> InObjectClass, TArray<UObject*>& OutObjects) const;
private:
	/**
	 * 开始适用对象
	 */
	static void Use(UObjectPooling* Pooling, UPoolItemHandle* Handle);
	/**
	 * 释放对象
	 */
	static void Collect(UObjectPooling* Pooling, UPoolItemHandle* Handle);

public:
	/**
	 * 释放对象
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool")
	static void Collect(UPoolItemHandle* Handle);


protected:
	/**
	 * 创建对象的方法
	 */
	virtual UObject* CreateObject();
	/**
	 * 开始使用的执行函数
	 */
	virtual void OnUse(UPoolItemHandle* Handle);
	/**
	 * 开始收集的执行函数
	 */
	virtual void OnCollect(UPoolItemHandle* Handle);
};
