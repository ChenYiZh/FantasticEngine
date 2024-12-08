// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FantasyEngineEditorSettings.generated.h"

/**
 * 
 */
UCLASS(config=FantasyEngineEditor, defaultconfig, meta=(DisplayName="Fantasy Engine Editor"))
class FANTASYENGINE_FRAMEWORK_EDITOR_API UFantasyEngineEditorSettings : public UObject
{
	GENERATED_BODY()

public:
	/** DataTable的文件名头 */
	UPROPERTY(config, EditAnywhere, Category="Table")
	FName DataTableHeader = TEXT("DT_Table");
	/** DataTable的存放目录 */
	UPROPERTY(config, EditAnywhere, Category="Table", meta=(LongPackageName))
	FDirectoryPath DataTableFolder;
	/** DataTable的结构文件名头 */
	UPROPERTY(config, EditAnywhere, Category="Table")
	FName DataTableStructHeader = TEXT("TS_Data");
	/** DataTable的结构存放目录 */
	UPROPERTY(config, EditAnywhere, Category="Table", meta=(LongPackageName))
	FDirectoryPath DataTableStructFolder;
	/** DataTable全局调用的系统类存放位置 */
	UPROPERTY(config, EditAnywhere, Category="Table", meta=(LongPackageName))
	FDirectoryPath DataTableSystemFolder;
	/** Excel 存放位置 */
	UPROPERTY(config, EditAnywhere, Category="Table")
	FDirectoryPath ExcelTablesFolder;
	/** Excel 转换工具*/
	UPROPERTY(config, EditAnywhere, Category="Table")
	FFilePath ConverterFilePath;
	/** Excel 转换工具的额外参数*/
	UPROPERTY(config, EditAnywhere, Category="Table")
	FString ConverterParams;
};
