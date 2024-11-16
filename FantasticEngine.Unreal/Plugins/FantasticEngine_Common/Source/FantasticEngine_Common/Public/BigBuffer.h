// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BigBuffer.generated.h"

/** Array64<uint8>结构 */
USTRUCT(Category="Fantastic Engine")
struct FANTASTICENGINE_COMMON_API FBigBuffer
{
	GENERATED_BODY()

public:
	FBigBuffer(): Buffer(MakeShared<TArray64<uint8>>())
	{
	}

	FBigBuffer(const TArray64<uint8>& InBuffer): Buffer(MakeShared<TArray64<uint8>>(InBuffer))
	{
	}

public:
	//UPROPERTY()
	TSharedRef<TArray64<uint8>> Buffer;
};
