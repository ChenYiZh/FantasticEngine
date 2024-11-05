// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "BlueprintBridgeUtilsBase.generated.h"

/** C++ <--> 蓝图 桥梁基类 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UBlueprintBridgeUtilsBase : public UWorldObject
{
	GENERATED_BODY()

public:
	/** 获取 C++ <--> 蓝图桥梁类 */
	UFUNCTION(BlueprintPure, DisplayName="BLueprint Bridge Utils",
		meta=(WorldContext="WorldContextObject", DeterminesOutputType=Class))
	static UBlueprintBridgeUtilsBase* GetBridge(const UObject* WorldContextObject,
	                                            TSubclassOf<UBlueprintBridgeUtilsBase> Class = nullptr);
};

/** 获取 C++ <--> 蓝图桥梁类 */
template <class T>
T* GetBridge(const UObject* WorldContextObject)
{
	return Cast<T>(UBlueprintBridgeUtilsBase::GetBridge(WorldContextObject));
}
