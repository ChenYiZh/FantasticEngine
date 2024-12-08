// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/EventSystem.h"
#include "UObject/Object.h"
#include "EventParam_Actor.generated.h"

/**
 * Event Param类
 */
UCLASS()
class FANTASYENGINE_FRAMEWORK_API UEventParam_Actor : public UEventParam
{
	GENERATED_BODY()
public:
	/**
	 * 透传Actor
	 */
	UPROPERTY(BlueprintReadWrite, Category="Event Param")
	AActor* Actor = nullptr;
};
