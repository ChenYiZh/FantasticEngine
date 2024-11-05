#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GlobalSettings.generated.h"

/** 全局选项类型 */
UENUM(BlueprintType)
enum class EGlobalOptionStyle :uint8
{
	/** 选项框 */
	CheckBox = 0,
	/** 滑动条 */
	Slider = 10,
	/** 下拉菜单 */
	DropdownList = 20
};

/** 全局选项的显示类型 */
UENUM(BlueprintType)
enum class EGlobalOptionDisplay :uint8
{
	/** 全平台显示 */
	AlwaysDisplay = 0,
	/** 不显示 */
	NeverDisplay = 2,
	/** 只在PC端显示 */
	OnlyOnPC = 5,
	/** 只在移动端上显示 */
	OnlyOnMobile = 10,
};

/** 全局设置选项词条 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FGlobalOption : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 显示名称 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText DisplayName;
	/** 选项类型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGlobalOptionStyle Style;
	/** 是否显示词条 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGlobalOptionDisplay Display = EGlobalOptionDisplay::AlwaysDisplay;
	/** 是否是开发用词条，发布时是否显示 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Development = false;
	/** 选项配置 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MultiLine=true))
	FText Configs;
	/** 可选信息 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MultiLine=true))
	FText Optional;
	/** 开始使用时调用Lua，传入 Item: UI 以及 Option: FGlobalOption */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MultiLine=true))
	FString GenerateByLua;
	/** 使用Lua来进行设置，传入 Item: UI 以及 Option: FGlobalOption, 必须包含 UI 的 Value 可用类型的返回值 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MultiLine=true))
	FString GetValueByLua;
	/** 使用Lua来进行设置，传入参数为 Item: UI 以及 Value */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MultiLine=true))
	FString SetValueByLua;
};

/**
 * 全局选项页签
 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FGlobalOptions : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 组别名称 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText DisplayName;
	/** 组别显示方案 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGlobalOptionDisplay Display = EGlobalOptionDisplay::AlwaysDisplay;
	/** 组别中的选项 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UDataTable> Options;
};

/**
 * 全局选项
 */
USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FGlobalSettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 选项框名称 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText DisplayName;
	/** 选项框显示方案 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGlobalOptionDisplay Display = EGlobalOptionDisplay::AlwaysDisplay;
	/** 选项框中的选项 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UDataTable> Options;
};
