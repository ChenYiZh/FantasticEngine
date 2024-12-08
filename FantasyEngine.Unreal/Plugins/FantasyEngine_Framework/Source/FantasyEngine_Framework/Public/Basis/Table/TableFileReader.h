// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableScheme.h"
#include "UObject/NoExportTypes.h"
#include "TableFileReader.generated.h"

/**
 * 
 */
class FANTASYENGINE_FRAMEWORK_API FTableReadTask : public FNonAbandonableTask
{
	//friend class FAsyncTask<TableReadTask>;
	friend class FAutoDeleteAsyncTask<FTableReadTask>;

private:
	//bool bIsLoading;
	class UTableFileReader* Reader;

	UTableScheme* Table;

	FTableReadTask(UTableFileReader* InReader, UTableScheme* InTable):
		Reader(InReader), Table(InTable)
	{
	}

public:
	void DoWork();
	// 固定写法，本类将作为函数参数
	FORCEINLINE TStatId GetStatId() const;
};


UCLASS()
class FANTASYENGINE_FRAMEWORK_API UTableFileReader : public UObject
{
	GENERATED_BODY()
	friend class FTableReadTask;

private:
	FThreadSafeBool bIsDone;
	UPROPERTY()
	FString Error;

private:
	UPROPERTY()
	int32 Shift;
	UPROPERTY()
	FString LocalMd5;
	UPROPERTY()
	FString OnlineMd5;
	UPROPERTY()
	bool bUseOnline;

public:
	void Read(UTableScheme* Table,
	          const FString& InLocalMd5,
	          const FString& InOnlineMd5,
	          const bool& bInUseOnline);
	bool IsDone() const;
	bool IsError() const;
	const FString& GetError() const;
};
