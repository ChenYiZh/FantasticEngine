// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "DataLanguage.generated.h"

/**
 * 多语言语言结构
 * Name应该为LanguageId
 */
USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FDataLanguage : public FTableRowBase
{
	GENERATED_BODY()

public:
	// /**
	//  * 多语言编号
	//  */
	// UPROPERTY(BlueprintReadOnly, EditAnywhere)
	// int32 LanguageId;
	/**
	 * 多语言的实际输出
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Text;
};
