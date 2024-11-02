// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/EnumDefines.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextUtility.generated.h"

/**
 * 文本辅助类
 */
UCLASS()
class FANTASYENGINE_FRAMEWORK_API UTextUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 存储组别 */
	UFUNCTION(BlueprintPure, Exec, Category="Language")
	static FString GetSectionName();

public:
	/** 区域码转枚举 */
	UFUNCTION(BlueprintCallable, Category="Language")
	static ELanguage::Type ConvertCodeToLanguage(const FString Code);

	/** 语言转区域码 */
	UFUNCTION(BlueprintCallable, Category="Language")
	static FString ConvertLanguageToCode(const ELanguage::Type Language);

	/** int转枚举 */
	UFUNCTION(BlueprintCallable, Category="Language")
	static ELanguage::Type ConvertIntToLanguage(const int32 InLanguage);


	/** 获取语言显示的名称 */
	UFUNCTION(BlueprintCallable, Category="Language")
	static FName GetLanguageDisplayName(const ELanguage::Type Language);

public:
	UFUNCTION(BlueprintCallable, Category="Language")
	static ELanguage::Type SetTextLanguage(ELanguage::Type NewLanguage);

	/** 获取文本语言 */
	UFUNCTION(BlueprintCallable, Category="Language")
	static ELanguage::Type GetTextLanguage();
};
