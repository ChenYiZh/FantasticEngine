// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ObjectFactory.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_FRAMEWORK_API UObjectFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Object Factory", meta=(WorldContext="WorldContextObject"))
	static TMap<UClass*, UObject*>& GetSingletonPool(const UObject* WorldContextObject);

public:
	// UFUNCTION(BlueprintCallable, Category="Object Factory", DisplayName="New",
	// 	meta = (DeterminesOutputType = "Class", DefaultToSelf="Outer", HidePin="Outer"))
	// static UObject* New(UObject* Outer, UClass* Class);

	// template <class T>
	// static T* New(UObject* Outer, UClass* Class)
	// {
	// 	return Cast<T>(New(Outer, Class));
	// }
	//
	// template <class T>
	// static T* New(UObject* Outer)
	// {
	// 	return Cast<T>(UObjectFactory::New(Outer, T::StaticClass()));
	// }
	//
	// template <class T>
	// static T* New()
	// {
	// 	return New<T>(nullptr);
	// }

	UFUNCTION(BlueprintCallable, Category="Object Factory", DisplayName="Add Component",
		meta = (DeterminesOutputType = "Class"))
	static UActorComponent* AddComponent(TSubclassOf<UActorComponent> Class, AActor* Actor);

	UFUNCTION(BlueprintPure, Category="Object Factory", DisplayName="Copy Component",
		meta = (DeterminesOutputType = "Class"))
	static USceneComponent* CopyComponent(USceneComponent* SrcComponent, TSubclassOf<USceneComponent> Class = nullptr);

	template <class T>
	static T* AddComponent(AActor* Actor)
	{
		if (Actor == nullptr)
		{
			return nullptr;
		}
		return Cast<T>(UObjectFactory::AddComponent(Actor, T::StaticClass()));
	}

	/**
	 * 获取单例
	 * @param bIncludeChild 如果是子类也可以被返回
	 */
	UFUNCTION(BlueprintPure, Category="Object Factory", DisplayName="Singleton",
		meta = (WorldContext="WorldContextObject", DeterminesOutputType = "Class"))
	static UObject* Singleton(const UObject* WorldContextObject, UClass* Class, const bool bIncludeChild = false);

	/**
	 * 删除物体
	 */
	UFUNCTION(BlueprintCallable, Category="Object Factory", DisplayName="Destroy")
	static void Destroy(UObject* Object);

public:
	/**
	 * 将物体放到Root上
	 */
	UFUNCTION(BlueprintCallable, Category="Object Factory")
	static void AddToRoot(UObject* Object);

	/**
	 * 将物体从Root上移除
	 */
	UFUNCTION(BlueprintCallable, Category="Object Factory")
	static void RemoveFromRoot(UObject* Object);

public:
	static void RemoveAllSingletons(const UObject* WorldContextObject);
};

//FANTASTICENGINE_FRAMEWORK_API TMap<UClass*, UObject*>& SingletonPool();

FANTASTICENGINE_FRAMEWORK_API void Destroy(UObject* Object);

/**
 * @param bIncludeChild 子类是不是也包含在里面
 */
template <class T>
FORCEINLINE T* Singleton(const UObject* WorldContextObject, bool bIncludeChild = false)
{
	return Cast<T>(UObjectFactory::Singleton(WorldContextObject, T::StaticClass(), bIncludeChild));
}
