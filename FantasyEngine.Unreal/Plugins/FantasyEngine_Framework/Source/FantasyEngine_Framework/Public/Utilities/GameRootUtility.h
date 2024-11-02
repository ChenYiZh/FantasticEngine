// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRoot.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameRootUtility.generated.h"

/** GameRoot 的拓展接口 */
UCLASS()
class FANTASYENGINE_FRAMEWORK_API UGameRootUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 判断运行模式是否是Debug */
	UFUNCTION(BlueprintPure, Exec, Category="Fantasy Engine|Game Root Utility")
	static bool IsDebug(const UObject* WorldContextObject)
	{
		return UGameRoot::IsDebug(WorldContextObject);
	}

	/** 判断是否可以执行初始化 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantasy Engine|Game Root Utility", meta=(WorldContext="WorldContextObject"))
	static bool IsInitialized(const UObject* WorldContextObject,UPARAM(ref) bool& bInitialized)
	{
		if (bInitialized) { return false; }
		if (const UGameRoot* Root = UGameRoot::GetRoot(WorldContextObject))
		{
			bInitialized = Root->Initialized();
			return bInitialized;
		}
		return false;
	}
};
