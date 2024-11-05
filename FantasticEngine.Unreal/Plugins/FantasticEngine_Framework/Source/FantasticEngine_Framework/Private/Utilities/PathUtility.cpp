// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/PathUtility.h"

// FString UPathUtility::BASE_PATH()
// {
// #if PLATFORM_ANDROID
// 		return FString();
// #elif  PLATFORM_IOS
// 		return FString();
// #elif PLATFORM_WINDOWS
// 	return FPaths::ConvertRelativePathToFull(FPlatformProcess::BaseDir(), FPaths::ProjectContentDir()).
// 		Replace(L"/", L"\\");;
// #endif
// 	return FString();
// }

FString UPathUtility::STREAMING_ASSETS_PATH()
{
	//TODO: 只读目录后期修改
	return FString("/Game/");
	//return FPaths::ProjectContentDir();
}

FString UPathUtility::WIDGETS_PATH()
{
	return RESOURCE_PATH() + FString("UI/Widgets/");
}

FString UPathUtility::TIPS_PATH()
{
	return RESOURCE_PATH() + FString("UI/TipWidgets/Tips/");
}

FString UPathUtility::Level_PATH()
{
	return FString("/Game/Maps/");
}

FString UPathUtility::PERSISTENT_DATA_PATH()
{
	return FPaths::ProjectSavedDir();
}

FString UPathUtility::RESOURCE_PATH()
{
	return STREAMING_ASSETS_PATH() + FString("Resources/");
}

FString UPathUtility::DATA_PATH()
{
	return PERSISTENT_DATA_PATH() + FString("Data/");
}

FString UPathUtility::TABLE_PATH()
{
	//return RESOURCE_PATH() + FString("Tables/");
	return FPaths::Combine(FPaths::ProjectContentDir(), TEXT("/Resources/Tables/"));
}

FString UPathUtility::TABLE_IDX_PATH()
{
	return FPaths::Combine(FPaths::ProjectContentDir(), TEXT("/Resources/assets.idx"));
}

FString UPathUtility::CombineBlueprintPath(const FString& RelativePath, const FString& Filename)
{
	return CheckBlueprintPath(CombineAssetPath(RelativePath, Filename));
}

FString UPathUtility::CheckBlueprintPath(const FString& FullPath)
{
	if (!FullPath.EndsWith(TEXT("_C")) && !FullPath.EndsWith(TEXT("_C'")))
	{
		if (FullPath.EndsWith(TEXT("'")))
		{
			return FullPath.Mid(0, FullPath.Len() - 1) + TEXT("_C'");
		}
		else
		{
			return FullPath + TEXT("_C");
		}
	}
	return FullPath;
}

FString UPathUtility::CombineAssetPath(const FString& RelativePath, const FString& Filename)
{
	FStringFormatOrderedArguments Args;
	Args.Add(RelativePath);
	Args.Add(Filename);
	return FString::Format(TEXT("{0}{1}.{1}"), Args);
}

// FString UPathUtility::LOG_PATH()
// {
// 	return PERSISTENT_DATA_PATH() + FString(FApp::GetProjectName()) + FString(".log");
// }

bool UPathUtility::ReadFileText(FString FullPath, FString& Result)
{
	return FFileHelper::LoadFileToString(Result, ToCStr(FullPath));
}

bool UPathUtility::WriteFileText(FString FullPath, FString Content)
{
	return FFileHelper::SaveStringToFile(Content, ToCStr(FullPath), FFileHelper::EEncodingOptions::ForceUTF8);
}

bool UPathUtility::AppendFileText(FString FullPath, FString Content)
{
	return FFileHelper::SaveStringToFile(Content, ToCStr(FullPath), FFileHelper::EEncodingOptions::ForceUTF8,
	                                     &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

FSoftObjectPath UPathUtility::ToSoftObjectPath(const FString LongPackagePath)
{
	return FSoftObjectPath(LongPackagePath);
}
