// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldObject.generated.h"

/** 自定义 Object ，可以安全调用 BlueprintFunctionLibrary */
UCLASS(BlueprintType, Blueprintable, Category="Fantastic Engine")
class FANTASTICENGINE_COMMON_API UWorldObject : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override
	{
#if WITH_EDITOR
		if (IsTemplate())
		{
			return nullptr;
		}
#endif

		return Super::GetWorld();
	}
};
