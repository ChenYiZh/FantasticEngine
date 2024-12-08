// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputMapping.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASYENGINE_VIRTUALINPUT_API UInputMapping : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetArrowUp();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetArrowDown();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetArrowLeft();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetArrowRight();

	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetActionUp();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetActionDown();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetActionLeft();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetActionRight();

	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetAxisMoveForward();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetAxisMoveRight();

	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetAxisLookUpRate();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetAxisTurnRate();

	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetAxisLookUp();
	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetAxisTurn();

	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetAxisWheel();

	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetLeftShoulder();

	UFUNCTION(BlueprintPure, Category="Input Mapping|Keys")
	static FName GetCtrl();

public:
	UFUNCTION(BlueprintCallable, Category="Input Mapping")
	static void BindBaseGamePadMapping();

	UFUNCTION(BlueprintCallable, Category="Input Mapping")
	static void BindARPGMapping();
};
