// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "Basis/Table/TableFileReader.h"
#include "Basis/Table/TableScheme.h"
#include "TableSystem.generated.h"

UENUM(BlueprintType)
enum class ETableLoadMode :uint8
{
	/**
	 * 默认
	 */
	Default = 0,
	/**
	 * 强制读 本地 表
	 */
	ForceLocal = 1,
	/**
	 * 强制读 线上 表
	 */
	ForceOnline = 5,
};

UENUM(BlueprintType)
enum class ETableSystemState :uint8
{
	Invalid = 0,
	Created = 1,
	Begin = 5,
	Loading = 10,
	Ready = 15,
};

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UTableSystem : public USystemBasis
{
	GENERATED_BODY()
	friend class FTableSystemBeginLoadingTask;
	friend UTableFileReader;

private:
	//UPROPERTY()
	int32 State = static_cast<int32>(ETableSystemState::Invalid);

private:
	UPROPERTY()
	TArray<UTableScheme*> Tables;
	UPROPERTY()
	TArray<UTableFileReader*> Readers;

public:
	void InitTables(const TArray<UTableScheme*>& InTables);
	void ReadTables();

	UFUNCTION(BlueprintPure)
	ETableSystemState GetState() const;

protected:
	virtual void OnTick_Implementation(float DeltaTime) override;

	virtual void OnQuit_Implementation() override;

public:
	virtual bool IsReady_Implementation() override;
};


class FANTASTICENGINE_FRAMEWORK_API FTableSystemBeginLoadingTask : FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FTableSystemBeginLoadingTask>;
	friend class UTableSystem;

private:
	UTableSystem* TableSystem;

public:
	FTableSystemBeginLoadingTask(UTableSystem* System): TableSystem(System)
	{
	}

public:
	/**
	 * 线程操作
	 */
	void DoWork();
	// 固定写法
	FORCEINLINE TStatId GetStatId() const;

private:
	/**
	 * 读取Md5信息
	 */
	void LoadMd5(TMap<FString, FString>& Md5Map, const FString& Uri, const bool& bIsLocal, FString& Content) const;
};
