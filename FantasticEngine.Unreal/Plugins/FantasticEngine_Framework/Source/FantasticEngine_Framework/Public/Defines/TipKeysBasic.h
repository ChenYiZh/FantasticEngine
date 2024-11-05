// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TipKeysBasic.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASTICENGINE_FRAMEWORK_API UTipKeysBasic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Tip Keys")
	static FString LOADING();
};
