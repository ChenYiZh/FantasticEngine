// Fill out your copyright notice in the Description page of Project Settings.


#include "Defines/Settings.h"

#include "Utilities/PathUtility.h"

FString USettings::GetFileName()
{
	return UPathUtility::GetSettingsFileName();
}

int32 USettings::SetInt(const FString Section, const FString Key, const int32 Value)
{
	GConfig->SetInt(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

float USettings::SetFloat(const FString Section, const FString Key, const float Value)
{
	GConfig->SetFloat(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

double USettings::SetDouble(const FString Section, const FString Key, const double Value)
{
	GConfig->SetDouble(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

bool USettings::SetBool(const FString Section, const FString Key, const bool Value)
{
	GConfig->SetBool(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

FString USettings::SetString(const FString Section, const FString Key, const FString Value)
{
	GConfig->SetString(ToCStr(Section), ToCStr(Key), ToCStr(Value), GetFileName());
	return Value;
}

void USettings::SetStringArray(const FString Section, const FString Key, TArray<FString>& Value)
{
	GConfig->SetArray(ToCStr(Section), ToCStr(Key), Value, GetFileName());
}

void USettings::SetStringArrayInSingleLine(const FString Section, const FString Key, TArray<FString>& Value)
{
	GConfig->SetSingleLineArray(ToCStr(Section), ToCStr(Key), Value, GetFileName());
}

FColor USettings::SetColor(const FString Section, const FString Key, const FColor Value)
{
	GConfig->SetColor(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

FVector2D USettings::SetVector2D(const FString Section, const FString Key, const FVector2D Value)
{
	GConfig->SetVector2D(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

FVector USettings::SetVector(const FString Section, const FString Key, const FVector Value)
{
	GConfig->SetVector(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

FVector4 USettings::SetVector4(const FString Section, const FString Key, const FVector4 Value)
{
	GConfig->SetVector4(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

FRotator USettings::SetRotator(const FString Section, const FString Key, const FRotator Value)
{
	GConfig->SetRotator(ToCStr(Section), ToCStr(Key), Value, GetFileName());
	return Value;
}

int32 USettings::GetInt(const FString Section, const FString Key, const int32 Default)
{
	int32 Result;
	if (GConfig->GetInt(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

float USettings::GetFloat(const FString Section, const FString Key, const float Default)
{
	float Result;
	if (GConfig->GetFloat(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

double USettings::GetDouble(const FString Section, const FString Key, const double Default)
{
	double Result;
	if (GConfig->GetDouble(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

bool USettings::GetBool(const FString Section, const FString Key, const bool Default)
{
	bool Result;
	if (GConfig->GetBool(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

FString USettings::GetString(const FString Section, const FString Key, const FString Default)
{
	FString Result;
	if (GConfig->GetString(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

void USettings::GetStringArray(const FString Section, const FString Key, TArray<FString>& OutData)
{
	GConfig->GetArray(ToCStr(Section), ToCStr(Key), OutData, GetFileName());
}

void USettings::GetStringArrayInSingleLine(const FString Section, const FString Key, TArray<FString>& OutData)
{
	GConfig->GetSingleLineArray(ToCStr(Section), ToCStr(Key), OutData, GetFileName());
}

FColor USettings::GetColor(const FString Section, const FString Key, const FColor Default)
{
	FColor Result;
	if (GConfig->GetColor(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

FVector2D USettings::GetVector2D(const FString Section, const FString Key, const FVector2D Default)
{
	FVector2D Result;
	if (GConfig->GetVector2D(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

FVector USettings::GetVector(const FString Section, const FString Key, const FVector Default)
{
	FVector Result;
	if (GConfig->GetVector(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

FVector4 USettings::GetVector4(const FString Section, const FString Key, const FVector4 Default)
{
	FVector4 Result;
	if (GConfig->GetVector4(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}

FRotator USettings::GetRotator(const FString Section, const FString Key, const FRotator Default)
{
	FRotator Result;
	if (GConfig->GetRotator(ToCStr(Section), ToCStr(Key), Result, GetFileName()))
	{
		return Result;
	}
	return Default;
}
