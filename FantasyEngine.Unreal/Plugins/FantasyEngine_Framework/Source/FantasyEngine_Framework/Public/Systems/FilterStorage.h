// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "Interfaces/ILevelFilter.h"
#include "Interfaces/IWidgetFilter.h"
#include "FilterStorage.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API UFilterStorage : public UWorldObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UObject*> WidgetFilters;

public:
	UFUNCTION(BlueprintCallable, Category="Filter Storage")
	static void AddWidgetFilter(UObject* Filter);
	UFUNCTION(BlueprintCallable, Category="Filter Storage")
	static void RemoveWidgetFilter(UObject* Filter);
	UFUNCTION(BlueprintPure, Category="Filter Storage")
	static TArray<UObject*> GetWidgetFilters(const UObject* WorldContextObject);

private:
	UPROPERTY()
	TArray<UObject*> LevelFilters;

public:
	UFUNCTION(BlueprintCallable, Category="Filter Storage")
	static void AddLevelFilter(UObject* Filter);
	UFUNCTION(BlueprintCallable, Category="Filter Storage")
	static void RemoveLevelFilter(UObject* Filter);
	UFUNCTION(BlueprintPure, Category="Filter Storage")
	static TArray<UObject*> GetLevelFilters(const UObject* WorldContextObject);
};
