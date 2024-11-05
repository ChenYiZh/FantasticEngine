// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/BlueprintBridgeUtilsBase.h"

#include "GameRoot.h"

UBlueprintBridgeUtilsBase* UBlueprintBridgeUtilsBase::GetBridge(const UObject* WorldContextObject,
                                                                TSubclassOf<UBlueprintBridgeUtilsBase> Class)
{
	return GetGameDefines<UGameDefines>(WorldContextObject)->GetBlueprintBridgeUtils();
}
