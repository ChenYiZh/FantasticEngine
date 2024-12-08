// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/LevelSystem.h"
#include "UObject/Interface.h"
#include "ILevelFilter.generated.h"

USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FLevelFilterConfig : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	 * 全局UI过滤器类型
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MustImplement="ILevelFilter"))
	TSubclassOf<UObject> Filter;
	/**
	 * 是否启用
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Enable = true;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UILevelFilter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FANTASYENGINE_FRAMEWORK_API IILevelFilter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool OnMapBeginToLoad(UMap* Scene);

	UFUNCTION(BlueprintNativeEvent)
	void OnMapLoaded(UMap* Scene);

	UFUNCTION(BlueprintNativeEvent)
	void OnMapLeft(UMap* Scene);
};
