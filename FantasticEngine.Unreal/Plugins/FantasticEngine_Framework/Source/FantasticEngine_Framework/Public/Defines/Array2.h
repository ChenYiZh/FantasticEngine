// Fill out your copyright notice in the Description page of Project Settings.
/*****************************************************************************
 *二位数组
*****************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Array2.generated.h"

/** String 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FStringArray
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> Array;
};

// /** int16 数组 */
// USTRUCT(BlueprintType)
// struct FANTASTICENGINE_FRAMEWORK_API FInt16Array
// {
// 	GENERATED_BODY()
//
// public:
// 	/** 数组对象 */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere)
// 	TArray<int16> Array;
// };

/** int32 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FInt32Array
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> Array;
};


/** int64 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FInt64Array
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int64> Array;
};


// /** uint16 数组 */
// USTRUCT(BlueprintType)
// struct FANTASTICENGINE_FRAMEWORK_API FUInt16Array
// {
// 	GENERATED_BODY()
//
// public:
// 	/** 数组对象 */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere)
// 	TArray<uint16> Array;
// };


// /** uint32 数组 */
// USTRUCT(BlueprintType)
// struct FANTASTICENGINE_FRAMEWORK_API FUInt32Array
// {
// 	GENERATED_BODY()
//
// public:
// 	/** 数组对象 */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere)
// 	TArray<uint32> Array;
// };

// /** uint64 数组 */
// USTRUCT(BlueprintType)
// struct FANTASTICENGINE_FRAMEWORK_API FUInt64Array
// {
// 	GENERATED_BODY()
//
// public:
// 	/** 数组对象 */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere)
// 	TArray<uint64> Array;
// };

/** float 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FFloatArray
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<float> Array;
};

/** double 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FDoubleArray
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<double> Array;
};

/** bool 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FBoolArray
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<bool> Array;
};

/** uint8 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FUInt8Array
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<uint8> Array;
};

// /** char 数组 */
// USTRUCT(BlueprintType)
// struct FANTASTICENGINE_FRAMEWORK_API FCharArray
// {
// 	GENERATED_BODY()
//
// public:
// 	/** 数组对象 */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere)
// 	FString Array;
// };

/** datetime 数组 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FDateTimeArray
{
	GENERATED_BODY()

public:
	/** 数组对象 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDateTime> Array;
};
