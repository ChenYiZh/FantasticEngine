// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/TextUtility.h"

#include "Defines/Settings.h"

FString UTextUtility::GetSectionName()
{
	return TEXT("Language");
}

ELanguage::Type UTextUtility::ConvertCodeToLanguage(const FString Code)
{
	if (Code.StartsWith("en"))
	{
		return ELanguage::English;
	}
	if (Code.StartsWith("ja"))
	{
		return ELanguage::Japanese;
	}
	if (Code.StartsWith("zh"))
	{
		if (Code.StartsWith("zh-Hant", ESearchCase::IgnoreCase)
			|| Code.EndsWith("TW", ESearchCase::IgnoreCase)
			|| Code.EndsWith("HK", ESearchCase::IgnoreCase))
		{
			return ELanguage::ChineseTraditional;
		}
		return ELanguage::ChineseSimplified;
	}
	return ELanguage::English;
}

FString UTextUtility::ConvertLanguageToCode(const ELanguage::Type Language)
{
	switch (Language)
	{
	case ELanguage::English: return TEXT("en");
	case ELanguage::Japanese: return TEXT("ja");
	case ELanguage::ChineseTraditional: return TEXT("zh-Hant");
	case ELanguage::ChineseSimplified: return TEXT("zh-CN");
	}
	return TEXT("en");
}

ELanguage::Type UTextUtility::ConvertIntToLanguage(const int32 InLanguage)
{
	FCultureRef Culture = FInternationalization::Get().GetCurrentLanguage();
	ELanguage::Type Language = ConvertCodeToLanguage(Culture->GetNativeLanguage());
	switch (InLanguage)
	{
	case 0: Language = ELanguage::ChineseSimplified;
		break;
	case 1: Language = ELanguage::ChineseTraditional;
		break;
	case 5: Language = ELanguage::English;
		break;
	case 10: Language = ELanguage::Japanese;
		break;
	}
	return Language;
}

FName UTextUtility::GetLanguageDisplayName(const ELanguage::Type Language)
{
	switch (Language)
	{
	case ELanguage::English: return TEXT("English");
	case ELanguage::Japanese: return TEXT("日本語");
	case ELanguage::ChineseTraditional: return TEXT("繁體中文");
	case ELanguage::ChineseSimplified: return TEXT("简体中文");
	}
	return TEXT("English");
}

ELanguage::Type UTextUtility::SetTextLanguage(ELanguage::Type NewLanguage)
{
	USettings::SetInt(GetSectionName(),TEXT("TextLanguage"), NewLanguage);
	FInternationalization::Get().SetCurrentLanguage(ConvertLanguageToCode(NewLanguage));
	return NewLanguage;
}

ELanguage::Type UTextUtility::GetTextLanguage()
{
	int32 Language = USettings::GetInt(GetSectionName(),TEXT("TextLanguage"), -1);
	return ConvertIntToLanguage(Language);
}
