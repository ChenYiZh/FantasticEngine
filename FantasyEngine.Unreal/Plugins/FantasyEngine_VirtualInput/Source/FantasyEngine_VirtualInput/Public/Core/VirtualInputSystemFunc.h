// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VirtualInputSystem.h"
#include "Widgets/VirtualJoystickWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VirtualInputSystemFunc.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASYENGINE_VIRTUALINPUT_API UVirtualInputSystemFunc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Input System", meta=(WorldContext="WorldContextObject"))
	static void InitInputSystem(const UObject* WorldContextObject, FVirtualInputConfig InConfig);
	UFUNCTION(BlueprintCallable, Category="Input System", meta=(WorldContext="WorldContextObject"))
	static void CreateVirtualJoysticks(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category="Input System", meta=(WorldContext="WorldContextObject"))
	static void RegistVirtualJoystickWidget(UVirtualJoystickWidget* VirtualJoystickDisplay);
	// static void CreateVirtualJoysticksWithDisplay(TSubclassOf<UVirtualJoystickWidget> VirtualJoystickDisplayClass);
	// UFUNCTION(BlueprintCallable, Category="Input System")
	// static void AddVirtualJoysticksDisplay(TSubclassOf<UVirtualJoystickWidget> VirtualJoystickDisplayClass);
	UFUNCTION(BlueprintCallable, Category="Input System", meta=(WorldContext="WorldContextObject"))
	static void RemoveAll(const UObject* WorldContextObject);
	// UFUNCTION(BlueprintCallable, Category="Input System")
	// static void SetVirtualJoysticksVisible(bool Visible);
};
