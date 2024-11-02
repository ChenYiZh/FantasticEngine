// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventIdsBasic.generated.h"

/**
 * 框架类的EventId取值范围 1 - 100
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UEventIdsBasic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Exec, Category="Event Ids")
	static int32 GET_READY();

	UFUNCTION(BlueprintPure, Exec, Category="Event Ids")
	static int32 SHOW_LOADING();

	UFUNCTION(BlueprintPure, Exec, Category="Event Ids")
	static int32 LOADING_PROGRESS();

	UFUNCTION(BlueprintPure, Exec, Category="Event Ids")
	static int32 HIDE_LOADING();

	UFUNCTION(BlueprintPure, Exec, Category="Event Ids")
	static int32 EID_SCREEN_SIZE_CHANGED();

	UFUNCTION(BlueprintPure, Exec, Category="Event Ids")
	static int32 EID_ACTOR_GO_DIE();
};
