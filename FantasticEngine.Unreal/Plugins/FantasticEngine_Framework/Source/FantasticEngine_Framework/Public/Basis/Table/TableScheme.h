// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TableScheme.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UTableScheme : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FString TableName;

public:
	virtual void BeginRead();
	
	UFUNCTION(BlueprintPure, Category="Table Scheme")
	const FString& GetTableName() const;
	
	void Parse(const FString& FileContent);

protected:
	virtual void Parse(const TArray<TArray<FString>>& FileLines)
	{
	}
};
