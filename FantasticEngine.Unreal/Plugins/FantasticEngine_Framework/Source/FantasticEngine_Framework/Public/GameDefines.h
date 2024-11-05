// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Basis/Tips/TipWidgets.h"
#include "Defines/EnumDefines.h"
#include "Defines/InputDefines.h"
#include "Systems/BlueprintBridgeUtilsBase.h"
#include "Systems/TableSystem.h"
#include "UObject/NoExportTypes.h"

#include "Misc/EngineVersionComparison.h"

#include "GameDefines.generated.h"

/** 命令行指令配置 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FGameDefineCommand
{
	GENERATED_BODY()

public:
	/** 命令行指令 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Command;

	/** 是否只在 Debug 的环境下执行 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOnlyDebug = false;
};

UENUM(BlueprintType)
enum class EPlayMode: uint8
{
	AUTO = 0,
	DEBUG = 10,
	RELEASE = 20,
};

/**
 * Framework配置信息
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UGameDefines : public UWorldObject
{
	GENERATED_BODY()
	friend class UFEGameInstance;
#pragma region 全局配置信息

private:
	// /** 判断是否是编辑器 **/
	// UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Configs")
	// bool bIsEditor = GIsEditor;

	/** 判断是否是编辑器 **/
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Configs")
	EPlayMode PlayMode = EPlayMode::AUTO;

	/** 在启动前执行的控制台指令 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Configs")
	TArray<FGameDefineCommand> ConsoleCommands;

	/** 蓝图<-->C++桥梁，在 Settings 里配置 **/
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Configs")
	UBlueprintBridgeUtilsBase* BlueprintBridgeUtils = nullptr;

public:
	/** 蓝图<-->C++桥梁 **/
	UFUNCTION(BlueprintPure, Category="Configs", DisplayName="Blueprint Bridge Utils",
		meta=(DeterminesOutputType=Class))
	UBlueprintBridgeUtilsBase* GetBlueprintBridgeUtils(TSubclassOf<UBlueprintBridgeUtilsBase> Class = nullptr) const;

	/** 获取在启动前执行的控制台指令 */
	UFUNCTION(BlueprintPure, Category="Configs", DisplayName="Console Commands")
	void GetConsoleCommands(TArray<FGameDefineCommand>& Commands) const;

private:
	/**
	 * 全局系统配置信息（FSystemConfig）
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Required|Configs")
	TObjectPtr<UDataTable> DT_SystemConfigs;

	/**
	 * UI配置信息（FWidgetConfig）
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Required|Configs")
	TObjectPtr<UDataTable> DT_WidgetConfigs;

	/**
	 * UI过滤器配置信息（FWidgetFilterConfig）
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Required|Configs")
	TObjectPtr<UDataTable> DT_WidgetFilterConfigs;

	/**
	 * Level过滤器配置信息（FLevelFilterConfig）
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Required|Configs")
	TObjectPtr<UDataTable> DT_LevelFilterConfigs;

	// /**
	//  * 游戏开始是调用的注册类
	//  */
	// UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Default")
	// TSubclassOf<UGlobalConfig> GlobalConfigClass = UGlobalConfig::StaticClass();

	/**
	 * Tips类
	 * */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-1), Category="Required|UI")
	TSubclassOf<UTipWidgets> TipWidgetsClass = UTipWidgets::StaticClass();

	/**
	 * 框架中全局的材质属性变量
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-100), Category="Required|Configs")
	TObjectPtr<UMaterialParameterCollection> MPC_Framework;

public:
	/**
	 * 全局系统配置信息（FSystemConfig）
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Required|Configs", DisplayName="Widget Configs")
	UDataTable* GetSystemConfigs() const;

	/**
	 * UI配置信息（FWidgetConfig）
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Required|Configs", DisplayName="Widget Configs")
	UDataTable* GetWidgetConfigs() const;

	/**
	 * UI过滤器配置信息（FWidgetFilterConfig）
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Required|Configs", DisplayName="Widget Configs")
	UDataTable* GetWidgetFilterConfigs() const;

	/**
	 * Level过滤器配置信息（FLevelFilterConfig）
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Required|Configs", DisplayName="Widget Configs")
	UDataTable* GetLevelFilterConfigs() const;

	// /**
	//  * 游戏开始是调用的注册类
	//  */
	// UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Default", DisplayName="Global Config Class")
	// TSubclassOf<UGlobalConfig> GetGlobalConfigClass() const;

	/**
	 * Tips类
	 * */
	UFUNCTION(BlueprintPure, Category="Required|UI", DisplayName="Tip Widgets Class")
	TSubclassOf<UTipWidgets> GetTipWidgetsClass() const;

	/**
	 * 框架中全局的材质属性变量
	 */
	UFUNCTION(BlueprintPure, Category="Required|Configs", DisplayName="MPC_Framework")
	UMaterialParameterCollection* GetMPC_Framework() const;

	/** 重新设置运行模式 **/
	void SetPlayMode(EPlayMode NewValue);

	/** 获取运行模式 **/
	UFUNCTION(BlueprintPure, Exec, Category="Configs")
	bool IsDebug() const;

	// /** 重新设置是否是Editor **/
	// void SetIsEditor(bool NewValue);

	/** 获取是否是编辑器 **/
	UFUNCTION(BlueprintPure, Exec, Category="GameDefines|Configs")
	static bool IsEditor();
#pragma endregion

#pragma region Audio

private:
	/**
	 * 音效配置信息
	 */
	UPROPERTY(EditAnywhere, Category="Fantastic Engine|Configs|Audio")
	TSubclassOf<class UAudioClassConfigs> AudioClassConfigs;

public:
	/**
	 * 鼠标显示类型
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Audio")
	TSubclassOf<UAudioClassConfigs> GetAudioClassConfigs() const;
#pragma endregion

#pragma region Input

private:
	/**
	 * 默认鼠标显示类型
	 */
	UPROPERTY(EditAnywhere, Category="Fantastic Engine|Configs|Default")
	EMouseType MouseType = EMouseType::Visible;

public:
	/**
	 * 鼠标显示类型
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Default")
	EMouseType GetMouseType() const;
	/**
	 * 设置鼠标显示类型
	 * @param NewMouseType 新的鼠标显示类型
	 * @param bChangeCache 是否修改缓存值
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Configs|Default")
	void SetMouseType(EMouseType NewMouseType, const bool bChangeCache = false);
#pragma endregion

#pragma region 适配刘海屏

private:
	/**
	 * 适配方案
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Default")
	EScreenAdapter ScreenAdapter = EScreenAdapter::Auto;

public:
	/**
	 * 刘海屏的适配分辨率偏移量
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Default")
	float HairScreenOffset = 80;
	/**
	 * 刘海屏的适配分辨率
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Default")
	FVector2D MaxHairScreenResolution = FVector2D(1920, 1080);
	/**
	 * 带鱼屏的适配分辨率
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Default")
	FVector2D MaxWideScreenResolution = FVector2D(2560, 1080);

public:
	/**
	 * 是否使用适配刘海屏
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantastic Engine|Configs")
	EScreenAdapter GetScreenAdapter() const;
	/**
	 * 是否使用适配刘海屏
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantastic Engine|Configs")
	bool UseHairScreenAdapter() const;
	/**
	 * 是否使用适配带鱼屏
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantastic Engine|Configs")
	bool UseWideScreenAdapter() const;
#pragma endregion

#pragma region 读表逻辑

private:
	/**
	 * 多语言表 (UDataTable<DataLanguage>)
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Table")
	TSoftObjectPtr<UDataTable> TableLanguage;

public:
	/**
	 * 获取多语言表
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantastic Engine|Configs|Table")
	const UDataTable* GetTableLanguage() const;
	/**
	 * 获取多语言表
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantastic Engine|Configs|Table")
	FText GetLanguage(int32 LanguageId) const;

private:
	/**
	 * 数据表加载模式
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Table")
	ETableLoadMode TableLoadMode;
	/**
	 * 线上表配置Url
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Table")
	FString TableIdxUrl;
	/**
	 * 线上表Url
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Table")
	FString TableUrl;
	/**
	 * 表解析偏移量
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Table")
	int32 TableShift = 2;

public:
	/**
	 * 数据表加载模式
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Table", DisplayName="Table Load Mode")
	ETableLoadMode GetTableLoadMode() const;
	/**
	 * 线上表配置Url
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Table", DisplayName="Table Idx Url")
	FString GetTableIdxUrl() const;
	/**
	 * 线上表Url
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Table", DisplayName="Table Url")
	FString GetTableUrl() const;
	/**
	 * 表解析偏移量
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Table", DisplayName="Table Shift")
	int32 GetTableShift() const;
#pragma endregion


#pragma region UI配置

private:
	/** 是否使用完整UI队列，如果不使用则遇到相同队列就将前一个相同UI移除 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|UI")
	bool bUseFullStack = true;

	/** Tips显示的层级 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|UI")
	int TipWidgetsZOrder = 100000;

	/** 默认起始层级 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|UI")
	int DefaultZOrder = 10;

	// /**
	// * 默认起始层级
	// */
	// UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Assets")
	// FString WidgetPath = "/Resources/Widgets";

public:
	/** 是否使用完整UI队列，如果不使用则遇到相同队列就将前一个相同UI移除 */
	UFUNCTION(Exec, Category="Fantastic Engine|Configs|UI")
	bool UseFullStack() const { return bUseFullStack; }

	/** Tips显示的层级 */
	UFUNCTION(Exec, Category="Fantastic Engine|Configs|UI", DisplayName="Tip Widgets Zorder")
	int GetTipWidgetsZOrder() const;

	/** 默认起始层级 */
	UFUNCTION(Exec, Category="Fantastic Engine|Configs|UI", DisplayName="Default Zorder")
	int GetDefaultZOrder() const;
#pragma endregion

#pragma region 平台信息

private:
	/** 平台 */
	UPROPERTY(VisibleAnywhere, Category="Fantastic Engine|Configs|Device")
	EPlatform Platform;

public:
	/** 获取平台 */
	UFUNCTION(Exec, BlueprintPure, Category="Fantastic Engine")
	EPlatform GetPlatform() const;
#pragma endregion

#pragma region 编辑器

public:
	/** 获取编辑器的id只有在编辑器下有用 */
	UFUNCTION(Exec, BlueprintPure, Category="Fantastic Engine", DisplayName="PlayInEditorID")
	static int32 GetPlayInEditorID()
	{
#if UE_VERSION_OLDER_THAN(5, 5, 0)
		return GPlayInEditorID;
#else
		return UE::GetPlayInEditorID();
#endif
	}
#pragma endregion

#pragma region GameDefine逻辑

public:
	/**
	 * 初始化时调用，主要做一些赋值操作
	 */
	virtual void Initialize();
	/**
	 * 初始化时调用，主要做一些赋值操作
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Initialize")
	void K2_OnInitialize();

private:
	/**
	 * 判断平台
	 */
	static EPlatform AnalysisPlatform();
#pragma endregion
};
