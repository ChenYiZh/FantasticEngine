// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DeviceUtility.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_FRAMEWORK_API UDeviceUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 是否是移动端 */
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"))
	static bool IsMobile(const UObject* WorldContextObject);

	/** Mac地址 */
	UFUNCTION(BlueprintPure, DisplayName="Mac Address")
	static FString MacAddress();

	/** 唯一Id地址 */
	UFUNCTION(BlueprintPure, DisplayName="Machine Id")
	static FString MachineId();
};
