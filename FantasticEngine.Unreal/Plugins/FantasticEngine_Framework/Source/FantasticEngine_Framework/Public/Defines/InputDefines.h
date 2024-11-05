// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "UObject/Object.h"
#include "InputDefines.generated.h"

/**
 * 输入操作的映射
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UInputDefines : public UObject
{
	GENERATED_BODY()

private:
	/**
	 * 映射类集合
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Input")
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

public:
#pragma region 初始化操作
	virtual void Initialize();
#pragma endregion
};
