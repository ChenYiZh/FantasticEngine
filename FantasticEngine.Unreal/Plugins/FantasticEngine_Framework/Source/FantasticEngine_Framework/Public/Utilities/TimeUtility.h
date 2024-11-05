// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimeUtility.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_FRAMEWORK_API UTimeUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 两个时间相减 */
	UFUNCTION(BlueprintPure, Category="Time Utility")
	static FTimespan Substract(const FDateTime& A, const FDateTime& B);

	/** 时间转换成yyyyMMddHHmmss */
	UFUNCTION(BlueprintPure, Category="Time Utility")
	static FString ToString_yyyyMMddHHmmss(const FDateTime& Time)
	{
		return Time.ToString(TEXT("%Y%m%d%H%M%S"));
	}

	/** 时间转换成HHmmss */
	UFUNCTION(BlueprintPure, Category="Time Utility")
	static FString ToString_HHmmss(const FDateTime& Time)
	{
		return Time.ToString(TEXT("%H%M%S"));
	}
	
	/** 时间转换成HH:mm:ss */
	UFUNCTION(BlueprintPure, Category="Time Utility")
	static FString ToString_HH_mm_ss(const FDateTime& Time)
	{
		return Time.ToString(TEXT("%H:%M:%S"));
	}

	/** 时间转换成yyyy/MM/dd HH:mm:ss */
	UFUNCTION(BlueprintPure, Category="Time Utility")
	static FString ToString_yyyy_MM_dd_HH_mm_ss(const FDateTime& Time)
	{
		return Time.ToString(TEXT("%Y/%m/%d %H:%M:%S"));
	}

	/** 时间转换成yyyy-MM-dd HH:mm:ss */
	UFUNCTION(BlueprintPure, Category="Time Utility")
	static FString ToString_yyyy_MM_dd_HH_mm_ss2(const FDateTime& Time)
	{
		return Time.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
	}
};
