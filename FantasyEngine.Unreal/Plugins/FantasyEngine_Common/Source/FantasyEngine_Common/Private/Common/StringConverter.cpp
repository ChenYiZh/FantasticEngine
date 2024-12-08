// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/StringConverter.h"

#include "Common/SizeUtility.h"

void UStringConverter::ToUTF8(const FString& Src, TArray<uint8>& Result)
{
	FTCHARToUTF8 UTF8String(*Src);
	int32 Size = UTF8String.Length();
	Result.SetNum(Size);
	FMemory::Memcpy(Result.GetData(), UTF8String.Get(), Size);
}

FString UStringConverter::FromUTF8(const uint8* Bytes, const int32& Length)
{
	FUTF8ToTCHAR TCHARData(reinterpret_cast<const ANSICHAR*>(Bytes), Length);
	return FString(TCHARData.Length(), TCHARData.Get());
}

FString UStringConverter::ConvertBool(bool InValue)
{
	return InValue ? FString("True") : FString("False");
}

FString UStringConverter::ConverVector(FVector InVector)
{
	return FString::Printf(TEXT("(%f, %f, %f)"), InVector.X, InVector.Y, InVector.Z);
}

FString UStringConverter::ConvertVector2D(FVector2D InVector)
{
	return FString::Printf(TEXT("(%f, %f)"), InVector.X, InVector.Y);
}

FString UStringConverter::FastEncrypt(const FString Src, const int32 Shift)
{
	//TArray<WIDECHAR> Dst;
	//Dst.SetNum(Src.Len());
	if (Src.IsEmpty() || Src.Len() == 0)
	{
		return Src;
	}
	TArray<TCHAR> Result;
	Result.SetNum(Src.Len());
	// TCHAR C = FastEncryptChar(Src[0], Shift);
	// if (!((C < '0' || C > '9') && C != '\t' && C != '\r' && C != '\n'))
	// {
	// 	Result.Append(&C);
	// }
	// else
	// {
	// 	TCHAR Null;
	// 	Result.Append(&Null);
	// }

	for (int32 i = 0; i < Src.Len(); i++)
	{
		Result[i] = FastEncryptChar(Src[i], Shift);
	}
	return FString(Result);
}

TCHAR UStringConverter::FastEncryptChar(const TCHAR& Char, const int32& Shift)
{
	TCHAR Result = static_cast<TCHAR>((Char + Shift) % WCHAR_MAX);
	return Result;
}

bool UStringConverter::ToInt16(const FString Src, int16& Result)
{
	if (!Src.IsNumeric())
	{
		Result = -1;
		return false;
	}
	Result = static_cast<int16>(FCString::Atoi(ToCStr(Src)));
	if (Src.StartsWith("-") && Result > 0)
	{
		Result *= -1;
	}
	return true;
}

bool UStringConverter::ToInt32(const FString Src, int32& Result)
{
	if (!Src.IsNumeric())
	{
		Result = -1;
		return false;
	}
	Result = FCString::Atoi(ToCStr(Src));
	if (Src.StartsWith("-") && Result > 0)
	{
		Result *= -1;
	}
	return true;
}

bool UStringConverter::ToInt64(const FString Src, int64& Result)
{
	if (!Src.IsNumeric())
	{
		Result = -1;
		return false;
	}
	Result = FCString::Atoi64(ToCStr(Src));
	return true;
}

bool UStringConverter::ToUInt16(const FString Src, uint16& Result)
{
	if (!Src.IsNumeric())
	{
		Result = 0;
		return false;
	}
	Result = static_cast<uint16>(FCString::Atoi(ToCStr(Src)));
	return true;
}

bool UStringConverter::ToUInt32(const FString Src, uint32& Result)
{
	if (!Src.IsNumeric())
	{
		Result = 0;
		return false;
	}
	Result = static_cast<uint32>(FCString::Atoi64(ToCStr(Src)));
	return true;
}

bool UStringConverter::ToUInt64(const FString Src, uint64& Result)
{
	if (!Src.IsNumeric())
	{
		Result = 0;
		return false;
	}
	//Base表示多少进制
	//https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/reference/strtoi64-wcstoi64-strtoi64-l-wcstoi64-l?view=msvc-170
	Result = FCString::Strtoui64(ToCStr(Src), nullptr, 10);
	return true;
}

bool UStringConverter::ToFloat(const FString Src, float& Result)
{
	if (!Src.IsNumeric())
	{
		Result = 0;
		return false;
	}
	Result = FCString::Atof(ToCStr(Src));
	if (Src.StartsWith("-") && Result > 0)
	{
		Result *= -1;
	}
	return true;
}

bool UStringConverter::ToDouble(const FString Src, double& Result)
{
	if (!Src.IsNumeric())
	{
		Result = -1;
		return false;
	}
	Result = FCString::Atod(ToCStr(Src));
	if (Src.StartsWith("-") && Result > 0)
	{
		Result *= -1;
	}
	return true;
}

void UStringConverter::ToBool(const FString Src, bool& Result)
{
	if (Src.IsNumeric())
	{
		Result = Src == "1";
	}
	else
	{
		Result = Src.ToLower() == "true";
	}
}

bool UStringConverter::ToUInt8(const FString Src, uint8& Result)
{
	if (!Src.IsNumeric())
	{
		Result = 0;
		return false;
	}
	Result = static_cast<uint8>(FCString::Atoi(ToCStr(Src)));
	return true;
}

void UStringConverter::ToChar(const FString Src, TCHAR& Result)
{
	Result = Src[0];
}

void UStringConverter::ToDateTime(const FString Src, FDateTime& Result)
{
	FString FixedString = Src.Replace(TEXT("-"), TEXT(" "));
	FixedString.ReplaceInline(TEXT(":"), TEXT(" "), ESearchCase::CaseSensitive);
	FixedString.ReplaceInline(TEXT("."), TEXT(" "), ESearchCase::CaseSensitive);
	// split up on a single delimiter
	TArray<FString> Tokens;
	FixedString.ParseIntoArray(Tokens, TEXT(" "), true);
	while (Tokens.Num() < 6)
	{
		Tokens.Insert("0", 0);
	}
	FDateTime::Parse(FString::Join(Tokens,TEXT("-")), Result);
}

void UStringConverter::ToStringArray(const FString Src, TArray<FString>& Result)
{
	Result.Empty();
	Src.ParseIntoArray(Result,TEXT(","), false);
}

void UStringConverter::ToInt16Array(const FString Src, TArray<int16>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToInt16(Array[i], Result[i]);
	}
}

void UStringConverter::ToInt32Array(const FString Src, TArray<int32>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToInt32(Array[i], Result[i]);
	}
}

void UStringConverter::ToInt64Array(const FString Src, TArray<int64>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToInt64(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt16Array(const FString Src, TArray<uint16>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt16(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt32Array(const FString Src, TArray<uint32>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt32(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt64Array(const FString Src, TArray<uint64>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt64(Array[i], Result[i]);
	}
}

void UStringConverter::ToFloatArray(const FString Src, TArray<float>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToFloat(Array[i], Result[i]);
	}
}

void UStringConverter::ToDoubleArray(const FString Src, TArray<double>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToDouble(Array[i], Result[i]);
	}
}

void UStringConverter::ToBoolArray(const FString Src, TArray<bool>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToBool(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt8Array(const FString Src, TArray<uint8>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt8(Array[i], Result[i]);
	}
}

void UStringConverter::ToCharArray(const FString Src, TArray<TCHAR>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToChar(Array[i], Result[i]);
	}
}

void UStringConverter::ToDateTimeArray(const FString Src, TArray<FDateTime>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	ToStringArray(Src, Array);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToDateTime(Array[i], Result[i]);
	}
}

void UStringConverter::ToStringArrayArray(const FString Src, TArray<TArray<FString>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToStringArray(Array[i], Result[i]);
	}
}

void UStringConverter::ToInt16ArrayArray(const FString Src, TArray<TArray<int16>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToInt16Array(Array[i], Result[i]);
	}
}

void UStringConverter::ToInt32ArrayArray(const FString Src, TArray<TArray<int32>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToInt32Array(Array[i], Result[i]);
	}
}

void UStringConverter::ToInt64ArrayArray(const FString Src, TArray<TArray<int64>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToInt64Array(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt16ArrayArray(const FString Src, TArray<TArray<uint16>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt16Array(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt32ArrayArray(const FString Src, TArray<TArray<uint32>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt32Array(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt64ArrayArray(const FString Src, TArray<TArray<uint64>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt64Array(Array[i], Result[i]);
	}
}

void UStringConverter::ToFloatArrayArray(const FString Src, TArray<TArray<float>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToFloatArray(Array[i], Result[i]);
	}
}

void UStringConverter::ToDoubleArrayArray(const FString Src, TArray<TArray<double>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToDoubleArray(Array[i], Result[i]);
	}
}

void UStringConverter::ToBoolArrayArray(const FString Src, TArray<TArray<bool>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToBoolArray(Array[i], Result[i]);
	}
}

void UStringConverter::ToUInt8ArrayArray(const FString Src, TArray<TArray<uint8>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToUInt8Array(Array[i], Result[i]);
	}
}

void UStringConverter::ToCharArrayArray(const FString Src, TArray<TArray<TCHAR>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToCharArray(Array[i], Result[i]);
	}
}

void UStringConverter::ToDateTimeArrayArray(const FString Src, TArray<TArray<FDateTime>>& Result)
{
	Result.Empty();
	TArray<FString> Array;
	Src.ParseIntoArray(Array,TEXT(";"), false);
	Result.SetNum(Array.Num());
	for (int32 i = 0; i < Array.Num(); i++)
	{
		ToDateTimeArray(Array[i], Result[i]);
	}
}
