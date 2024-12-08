// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PathUtility.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYENGINE_FRAMEWORK_API UPathUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 外部主路径 */
	UFUNCTION(BlueprintPure, Category="Path", DisplayName="Project Path")
	static FString GetProjectPath() { return FPaths::ProjectDir(); }

	/** 外部主路径下的保存路径 */
	UFUNCTION(BlueprintPure, Category="Path", DisplayName="Saved Path")
	static FString GetSavedPath() { return GetProjectPath() + TEXT("Saved/"); }

	/** 外部主路径下的配置文件路径 */
	UFUNCTION(BlueprintPure, Category="Path", DisplayName="Saved Path")
	static FString GetConfigPath() { return GetSavedPath() + TEXT("Config/"); }

	/** 外部主路径下的日志文件路径 */
	UFUNCTION(BlueprintPure, Category="Path", DisplayName="Saved Path")
	static FString GetLogPath() { return GetSavedPath() + TEXT("Logs/"); }

	/** 配置文件路径 */
	UFUNCTION(BlueprintPure, Category="Path")
	static FString GetSettingsFileName() { return FPaths::Combine(GetConfigPath(), FString("Settings.ini")); }

	UFUNCTION(BlueprintPure, Category="Path")
	static FString STREAMING_ASSETS_PATH();

	UFUNCTION(BlueprintPure, Category="Path")
	static FString WIDGETS_PATH();

	UFUNCTION(BlueprintPure, Category="Path")
	static FString TIPS_PATH();

	UFUNCTION(BlueprintPure, Category="Path")
	static FString Level_PATH();

	UFUNCTION(BlueprintPure, Category="Path")
	static FString PERSISTENT_DATA_PATH();

	UFUNCTION(BlueprintPure, Category="Path")
	static FString RESOURCE_PATH();

	UFUNCTION(BlueprintPure, Category="Path")
	static FString DATA_PATH();

	/**
	 * 读表路径
	 */
	UFUNCTION(BlueprintPure, Category="Path")
	static FString TABLE_PATH();
	/**
	 * 表配置路径
	 */
	UFUNCTION(BlueprintPure, Category="Path")
	static FString TABLE_IDX_PATH();

	UFUNCTION(BlueprintCallable, Category="Path")
	static FString CombineBlueprintPath(const FString& RelativePath, const FString& Filename);

	UFUNCTION(BlueprintCallable, Category="Path")
	static FString CheckBlueprintPath(const FString& FullPath);

	UFUNCTION(BlueprintCallable, Category="Path")
	static FString CombineAssetPath(const FString& RelativePath, const FString& Filename);

	// UFUNCTION(BlueprintPure, Category="Path")
	// static FString LOG_PATH();

	UFUNCTION(BlueprintCallable, Category="IO")
	static bool ReadFileText(FString FullPath, FString& Result);

	UFUNCTION(BlueprintCallable, Category="IO")
	static bool WriteFileText(FString FullPath, FString Content);

	UFUNCTION(BlueprintCallable, Category="IO")
	static bool AppendFileText(FString FullPath, FString Content);

public:
	/** 长路径转资源 */
	UFUNCTION(BlueprintPure, Exec, Category="Path")
	static FSoftObjectPath ToSoftObjectPath(const FString LongPackagePath);
};
