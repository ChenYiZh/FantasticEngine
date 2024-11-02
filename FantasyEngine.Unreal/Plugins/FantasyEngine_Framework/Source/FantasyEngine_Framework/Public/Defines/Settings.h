// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Settings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API USettings : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FString GetFileName();

public:
	/** 保存int值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static int32 SetInt(const FString Section, const FString Key, const int32 Value);
	/** 保存Float值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static float SetFloat(const FString Section, const FString Key, const float Value);
	/** 保存Double值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static double SetDouble(const FString Section, const FString Key, const double Value);
	/** 保存Bool值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static bool SetBool(const FString Section, const FString Key, const bool Value);
	/** 保存String值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static FString SetString(const FString Section, const FString Key, const FString Value);

	/** 保存String数组 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static void SetStringArray(const FString Section, const FString Key, UPARAM(ref) TArray<FString>& Value);
	/** 在一行里保存String数组 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static void SetStringArrayInSingleLine(const FString Section, const FString Key,
	                                       UPARAM(ref) TArray<FString>& Value);
	/** 保存颜色值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static FColor SetColor(const FString Section, const FString Key, const FColor Value);
	/** 保存二维向量值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static FVector2D SetVector2D(const FString Section, const FString Key, const FVector2D Value);
	/** 保存三维向量值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static FVector SetVector(const FString Section, const FString Key, const FVector Value);
	/** 保存四维向量值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static FVector4 SetVector4(const FString Section, const FString Key, const FVector4 Value);
	/** 保存旋转值 **/
	UFUNCTION(BlueprintCallable, Category="Settings")
	static FRotator SetRotator(const FString Section, const FString Key, const FRotator Value);

public:
	/** 获取int值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static int32 GetInt(const FString Section, const FString Key, const int32 Default = 0);
	/** 获取Float值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static float GetFloat(const FString Section, const FString Key, const float Default = 0.f);
	/** 获取Double值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static double GetDouble(const FString Section, const FString Key, const double Default = 0.0);
	/** 获取Bool值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static bool GetBool(const FString Section, const FString Key, const bool Default = false);
	/** 获取String值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static FString GetString(const FString Section, const FString Key, const FString Default = TEXT(""));

	/** 获取String数组 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static void GetStringArray(const FString Section, const FString Key, TArray<FString>& OutData);
	/** 在一行里获取String数组 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static void GetStringArrayInSingleLine(const FString Section, const FString Key,
	                                       TArray<FString>& OutData);
	/** 获取颜色值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static FColor GetColor(const FString Section, const FString Key, const FColor Default = FColor::White);
	/** 获取二维向量值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static FVector2D GetVector2D(const FString Section, const FString Key,
	                             const FVector2D Default = FVector2D::ZeroVector);
	/** 获取三维向量值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static FVector GetVector(const FString Section, const FString Key, const FVector Default = FVector::ZeroVector);
	/** 获取四维向量值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static FVector4 GetVector4(const FString Section, const FString Key, const FVector4 Default = FVector4());
	/** 获取旋转值 **/
	UFUNCTION(BlueprintPure, Category="Settings")
	static FRotator GetRotator(const FString Section, const FString Key,
	                           const FRotator Default = FRotator::ZeroRotator);
};
