// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include <mutex>

#include "CoreMinimal.h"
#include "Chaos/DebugDrawQueue.h"
#include "Basis/SystemBasis.h"
#include "EventSystem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UEventParam : public UWorldObject
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FEventCallback, UEventParam*, Param);

USTRUCT()
struct FANTASYENGINE_FRAMEWORK_API FEventMessagePair
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int EventId;
	UPROPERTY()
	UEventParam* Param;
};

USTRUCT()
struct FANTASYENGINE_FRAMEWORK_API FEventCallbackArray
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FEventCallback> Callbacks;
};

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API UEventSystem : public USystemBasis
{
	GENERATED_BODY()

private:
	// TSharedPtr<TQueue<TSharedPtr<TPair<int32, UEventParam*>, ESPMode::NotThreadSafe>>, ESPMode::NotThreadSafe> Messages
	// 	= MakeShared<TQueue<TSharedPtr<TPair<int32, UEventParam*>, ESPMode::NotThreadSafe>>, ESPMode::NotThreadSafe>();
	// TSharedPtr<TMap<int32, TSharedPtr<TArray<TSharedPtr<FEventCallback, ESPMode::NotThreadSafe>>,
	//                                   ESPMode::NotThreadSafe>>, ESPMode::NotThreadSafe> Callbacks
	// 	= MakeShared<TMap<int32, TSharedPtr<TArray<TSharedPtr<FEventCallback, ESPMode::NotThreadSafe>>,
	// 	                                    ESPMode::NotThreadSafe>>, ESPMode::NotThreadSafe>();

	UPROPERTY()
	TArray<FEventMessagePair> Messages;
	UPROPERTY()
	TMap<int32, FEventCallbackArray> Callbacks;

	//std::mutex Lock;
	//UPROPERTY()
	FCriticalSection Mutex;

public:
	virtual bool EnableTickBeforeInitialized_Implementation() override;

public:
	UFUNCTION(BlueprintCallable, DisplayName="Regist Event")
	void RegistEventImpl(int32 EventId, FEventCallback Callback);
	FEventCallback RegistEventNative(const UObject* WorldContextObject, int32 EventId, UObject* InObject,
	                                 const FName& InFunctionName);

	UFUNCTION(BlueprintCallable, DisplayName="Unregist Event")
	void UnregistEventImpl(int32 EventId, FEventCallback Callback);
	void UnregistEventNative(int32 EventId, UObject* InObject, const FName& InFunctionName);

	UFUNCTION(BlueprintCallable, DisplayName="Send Event")
	void SendEventImpl(int32 EventId, UEventParam* Param = nullptr);

public:
	virtual void OnTick_Implementation(float DeltaTime) override;

	virtual void OnQuit_Implementation() override;

private:
	void ExecuteMessage(const FEventMessagePair& Message);

public:
	UFUNCTION(BlueprintCallable, Category="Event System", meta=(WorldContext="WorldContextObject"),
		DisplayName="Regist Event")
	static void K2_RegistEvent(const UObject* WorldContextObject, int32 EventId, FEventCallback Callback);

	UFUNCTION(BlueprintCallable, Category="Event System", meta=(WorldContext="WorldContextObject"),
		DisplayName="Unregist Event")
	static void K2_UnregistEvent(const UObject* WorldContextObject, int32 EventId, FEventCallback Callback);

	UFUNCTION(BlueprintCallable, Category="Event System", meta=(WorldContext="WorldContextObject"))
	static void SendEvent(const UObject* WorldContextObject, int32 EventId, UEventParam* Param = nullptr);

public:
	static FEventCallback RegistEvent(const UObject* WorldContextObject, int32 EventId, UObject* InObject,
	                                  const FName& InFunctionName);

	static void UnregistEvent(const UObject* WorldContextObject, int32 EventId, UObject* InObject,
	                          const FName& InFunctionName);
};
