// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRoot.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "FEGameInstance.generated.h"

/**
 * 全局类
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UFEGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	bool bInited = false;
	//inline static UDefaultGameInstance* Instance = nullptr;

public:
	/**
	 * 全局类
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Fantasy Engine")
	UGameRoot* GameRoot;

public:
private:
	/**
	 * 初始化全局主类
	 * 2024/04/29 ChenYiZh: 改为私有自动执行
	 */
	//UFUNCTION(BlueprintCallable, Category="Fantasy Engine", meta=( WorldContext="WorldContextObject"))
	static void InitGameRoot(const UObject* WorldContextObject);

	// /**
	//  * 获取全局类
	//  */
	// UFUNCTION(BlueprintCallable, Category="Fantasy Engine")
	// static UDefaultGameInstance* GetInstance();

public:
	virtual void PostInitProperties() override;
	virtual void Init() override;
	virtual void OnStart() override;
	virtual void Shutdown() override;
	virtual void BeginDestroy() override;

public:
	/**
	 * ChenYiZh 2023/5/21: 控制台指令
	 */
	UFUNCTION(Exec)
	void Cmd(const FString Cmd);

#pragma region Command
	///控制台函数定义
public:
	/**
	 * 自定义命令需要实现获取行代码
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString CustomCommand(const FString& Cmd);
#pragma endregion
};
