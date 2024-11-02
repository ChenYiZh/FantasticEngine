// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuAction_GenerateTables.generated.h"

UENUM()
enum class EHeaderType:uint8
{
	STRING = 0,
	SHORT = 1,
	INT = 2,
	LONG = 3,
	USHORT = 4,
	UINT = 5,
	ULONG = 6,
	FLOAT = 7,
	DOUBLE = 8,
	BOOL = 9,
	BYTE = 10,
	CHAR = 11,
	DATE = 12,
	TIME = 13,
	DATETIME = 14,

	ARRAY_STRING = 100,
	ARRAY_SHORT = 101,
	ARRAY_INT = 102,
	ARRAY_LONG = 103,
	ARRAY_USHORT = 104,
	ARRAY_UINT = 105,
	ARRAY_ULONG = 106,
	ARRAY_FLOAT = 107,
	ARRAY_DOUBLE = 108,
	ARRAY_BOOL = 109,
	ARRAY_BYTE = 110,
	ARRAY_CHAR = 111,
	ARRAY_DATE = 112,
	ARRAY_TIME = 113,
	ARRAY_DATETIME = 114,

	ARRAY_ARRAY_STRING = 200,
	ARRAY_ARRAY_SHORT = 201,
	ARRAY_ARRAY_INT = 202,
	ARRAY_ARRAY_LONG = 203,
	ARRAY_ARRAY_USHORT = 204,
	ARRAY_ARRAY_UINT = 205,
	ARRAY_ARRAY_ULONG = 206,
	ARRAY_ARRAY_FLOAT = 207,
	ARRAY_ARRAY_DOUBLE = 208,
	ARRAY_ARRAY_BOOL = 209,
	ARRAY_ARRAY_BYTE = 210,
	ARRAY_ARRAY_CHAR = 211,
	ARRAY_ARRAY_DATE = 212,
	ARRAY_ARRAY_TIME = 213,
	ARRAY_ARRAY_DATETIME = 214
};

USTRUCT()
struct FHeaderInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName Name;
	UPROPERTY()
	EHeaderType Type;
	UPROPERTY()
	FString Description;
};

USTRUCT()
struct FTableInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName Name;
	UPROPERTY()
	TArray<FHeaderInfo> Headers;
};

class FMenuAction_GenerateTables
{
public:
	FString RootPath;
	FString ExeFilePath;
	FString ExeFolder;
	FString JsonFilePath;
	FString TableStructPath;
	FString TablePath;
	FName TableStructHeader;
	FName TableHeader;

public:
	/** 读取并加载表 */
	void GenerateTables();

private:
	/** 用C#创建表 */
	bool MakeTables();
	/** 读取表结构 */
	bool LoadJson(TArray<FTableInfo>& Tables);
	/** 整理表结构 */
	class UUserDefinedStruct* CheckTableStructure(const FTableInfo& Table, bool& bExists) const;
	/** 整理表数据 */
	class UDataTable* CheckTable(const FTableInfo& TableInfo, UUserDefinedStruct* TableStruct, bool& bExists);
	/** 获取属性类型 */
	static FEdGraphPinType GetFieldType(const EHeaderType& FieldType);
	/** 数据转String */
	static void ConvertJson(const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>>& JsonWriter,
	                        const EHeaderType& FieldType,
	                        const TCHAR* Key, const FString& Value);
	/** 创建蓝图可以调用的类型信息 */
	static void CreateBlueprintFunction(const TMap<FName, UDataTable*>& Tables);
};
