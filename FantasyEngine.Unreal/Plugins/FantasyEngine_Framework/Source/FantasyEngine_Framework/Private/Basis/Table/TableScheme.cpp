// Fill out your copyright notice in the Description page of Project Settings.


#include "Basis/Table/TableScheme.h"

void UTableScheme::BeginRead()
{
}

const FString& UTableScheme::GetTableName() const
{
	return TableName;
}

void UTableScheme::Parse(const FString& FileContent)
{
	TArray<FString> Lines;
	FileContent.ParseIntoArrayLines(Lines, false);
	TArray<TArray<FString>> Results;
	for (const FString& Line : Lines)
	{
		TArray<FString> Values;
		Line.ParseIntoArray(Values, TEXT("\t"), false);
		Results.Add(Values);
	}
	if (Results.Num() > 0)
	{
		Parse(Results);
	}
}
