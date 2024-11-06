// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FEUtilities.generated.h"

/**
 * 
 */
UCLASS(Category="Fantastic Engine")
class FANTASTICENGINE_COMMON_API UFEUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 拉起读取对话框
	 * @param DialogTitle 
	 * @param FileTypes *.*|*.jpg|...
	 * @param Filenames 选取的文件列表
	 * @param bMultiSelect 是否允许多选
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Utilities")
	static void OpenFileDialog(FString DialogTitle, const FString& FileTypes,
	                           TArray<FString>& Filenames, bool bMultiSelect = false);

	/**
	 * 拉起保存对话框
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param DefaultFile 默认文件名
	 * @param FileTypes *.*|*.jpg|...
	 * @param Filename 返回需要保存的文件路径
	 * @return 是否选取了
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Utilities")
	static bool SaveFileDialog(FString DialogTitle,
	                           FString DefaultPath, FString DefaultFile, const FString& FileTypes,
	                           FString& Filename);

private:
	static bool FileDialogShared(bool bSave, const void* ParentWindowHandle, const FString& DialogTitle,
	                             const FString& DefaultPath, const FString& DefaultFile,
	                             const FString& FileTypes,
	                             uint32 Flags, TArray<FString>& OutFilenames, int32& OutFilterIndex);
};
