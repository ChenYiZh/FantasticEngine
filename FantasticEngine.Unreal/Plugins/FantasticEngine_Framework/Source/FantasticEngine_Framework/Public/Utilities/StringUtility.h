// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringUtility.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_FRAMEWORK_API UStringUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="String")
	static void CheckCommands(FString Text, TArray<FString>& Commands);

	UFUNCTION(BlueprintCallable, Category="Montage")
	static void CheckMontageNotify(FName Name, FName& Notify, TArray<FString>& Commands);
};
