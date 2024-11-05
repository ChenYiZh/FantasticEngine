// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TransparentParameters.generated.h"

/**
 * 默认透传参数
 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FTransparentParameters
{
	GENERATED_BODY()
public:
	// /**
	//  * 是否有效
	//  */
	//bool bValid = true;

	// public:
	// 	/**
	// 	 * 设置是否有效
	// 	 */
	// 	void SetValid(bool bValid)
	// 	{
	// 		bIsValid = bValid;
	// 	}

	// /**
	//  * 有效值判断
	//  */
	// bool IsValid() const
	// {
	// 	return bValid;
	// }

	// /**
	//  * 判断是否一致
	//  */
	// bool Equals(const FTransparentParameters& Other)
	// {
	// 	if (!bValid)
	// 	{
	// 		return !Other.bValid;
	// 	}
	// 	if (!Other.bValid)
	// 	{
	// 		return false;
	// 	}
	// 	return this == &Other;
	// }

public:
	/**
	 * 整型标识
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ID = INDEX_NONE;
	/**
	 * 名称标记
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Tag = NAME_None;
	/**
	 * 透传一些整数型
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> Integers;
	/**
	 * 透传一些浮点型
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<double> Numbers;
	/**
	 * 透传一些文本
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> Contents;
	/**
	 * 透传一些Class类
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UClass*> OptionalClasses;
	/**
	 * 透传一些Object类
	 */
	UPROPERTY(BlueprintReadWrite)
	TArray<UObject*> OptionalObjects;
	/**
	 * 透传Actor
	 */
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> OptionalActors;
};