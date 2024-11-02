// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObject.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Utilities/ObjectFactory.h"
#include "SystemBasis.generated.h"

USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FSystemConfig : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	 * 全局系统类类型
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class USystemBasis> SystemClass;
	/**
	 * 是否启用
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Enable = true;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API USystemBasis : public UWorldObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool EnableTickBeforeInitialized();
	virtual bool EnableTickBeforeInitialized_Implementation();

public:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool IsReady();
	virtual bool IsReady_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInitialize();
	virtual void OnInitialize_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnReady();
	virtual void OnReady_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLogin();
	virtual void OnLogin_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLogout();
	virtual void OnLogout_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnReset();
	virtual void OnReset_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTick(float DeltaTime);
	virtual void OnTick_Implementation(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEverySecond();
	virtual void OnEverySecond_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnApplicationFocus(bool Focus);
	virtual void OnApplicationFocus_Implementation(bool Focus);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnApplicationPause(bool Pause);
	virtual void OnApplicationPause_Implementation(bool Pause);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnQuit();
	virtual void OnQuit_Implementation();
};
