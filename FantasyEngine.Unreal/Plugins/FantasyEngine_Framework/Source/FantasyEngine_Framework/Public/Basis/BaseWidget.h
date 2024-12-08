// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Defines/EnumDefines.h"
#include "Systems/UIFactory.h"
#include "Misc/EngineVersionComparison.h"
#include "BaseWidget.generated.h"

/************************************************************
 *注册模板
USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FWidgetConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TEnumAsByte<...> WidgetKeyName;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WidgetPath;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EUILevel UILevel;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool Transparent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimConfig AnimConfig;
};
***************************************************************/

UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UMiniWidget : public UUserWidget
{
	GENERATED_BODY()
#if WITH_EDITORONLY_DATA

private:
	/**
	 * 显示的窗口栏名称
	 * BaseWidget无效
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, DisplayPriority=-1), Category="Config")
	FText ItemPaletteCategory = NSLOCTEXT("UI", "UIItem", "UIItem");;
#endif

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

public:
	/**
	 * 创建时调用，赋值专用 
	 */
	virtual void NativeOnCreate();

protected:
	/**
	 * 创建时调用，赋值专用
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCreate();

public:
	/**
	 * 打开面板调用
	 */
	virtual void NativeOnWidgetOpen(UWidgetParam* Param);

protected:
	/**
	 * 打开面板调用
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnWidgetOpen(UWidgetParam* Param);

public:
	/**
	 * 显示动画执行完毕调用
	 */
	virtual void NativeOnShow();

protected:
	/**
	 * 显示动画执行完毕调用
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnShow();

public:
	/**
	 * 隐藏后执行
	 */
	virtual void NativeOnHide();

protected:
	/**
	 * 隐藏后执行
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHide();

public:
	/**
	 * 面板关闭后执行
	 */
	virtual void NativeOnWidgetClosed();

protected:
	/**
	 * 面板关闭后执行
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnWidgetClosed();

public:
	/**
	 * 帧事件
	 */
	virtual void NativeOnTick(float DeltaTime);
	/**
	 * 秒事件
	 */
	virtual void NativeOnEverySecond();

protected:
	/**
	 * 帧事件
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTick(float DeltaTime);

	/**
	 * 秒事件
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEverySecond();

public:
	/**
	 * 获取所有内部的Widgets 
	 */
	UFUNCTION(BlueprintCallable)
	void GetAllWidgets(TArray<UWidget*>& Widgets);

public:
	/**
	 * 多语言读取
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TranslateUIText();
};

/**
 * 基础需要UI使用大Widget
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UBaseWidget : public UMiniWidget
{
	GENERATED_BODY()

	friend class UUIFactory;

private:
	UPROPERTY()
	float Seconds = 0;
	UPROPERTY()
	FName KeyName = NAME_None;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

public:
	UFUNCTION(BlueprintPure)
	FName GetKeyName() const;
	void SetKeyName(const FName& InKeyName);

private:
	/** 是否根据刘海屏进行适配 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Config")
	EScreenAdapter ScreenAdapter = EScreenAdapter::Auto;

public:
	/** 打开这个界面时的鼠标状态 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config")
	EMouseType MouseType = EMouseType::Default;

public:
	/**
	 * 在播放显示动画前调用
	 * @param AnimIn 
	 * @param AnimInSeconds 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginShow(UPARAM(ref) EUIAnimType& AnimIn,UPARAM(ref) float& AnimInSeconds);
	virtual void BeginShow_Implementation(EUIAnimType& AnimIn, float& AnimInSeconds);

	/**
	 * 自定义显示动画，每帧执行，传入0-1的值，总时长需要预先设定
	 * 返回True说明动画执行完毕。可作为检测使用
	 * @param Alpha 差值，与总时间对应
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CustomAnimInLoop(float Alpha);
	/**
	* 自定义显示动画，每帧执行，传入0-1的值，总时长需要预先设定
	* 返回True说明动画执行完毕。可作为检测使用
	* @param Alpha 差值，与总时间对应
	*/
	virtual void CustomAnimInLoop_Implementation(float Alpha);

	/**
	 * 在播放隐藏动画前调用
	 * @param AnimOut 
	 * @param AnimOutSeconds 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginHide(UPARAM(ref) EUIAnimType& AnimOut,UPARAM(ref) float& AnimOutSeconds);
	virtual void BeginHide_Implementation(EUIAnimType& AnimOut, float& AnimOutSeconds);

	/**
	* 自定义隐藏动画，每帧执行，传入0-1的值，总时长需要预先设定
	* 返回True说明动画执行完毕。可作为检测使用
	* @param Alpha 差值，与总时间对应
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CustomAnimOutLoop(float Alpha);
	/**
	* 自定义隐藏动画，每帧执行，传入0-1的值，总时长需要预先设定
	* 返回True说明动画执行完毕。可作为检测使用
	* @param Alpha 差值，与总时间对应
	*/
	virtual void CustomAnimOutLoop_Implementation(float Alpha);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Close();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	/** 修改Widget显示大小 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Resize();

	/** 获取Widget最后固定的Anchor边距 */
	UFUNCTION(BlueprintCallable)
	FAnchors GetStableAnchors();

	/** Widget是否使用刘海屏 */
	UFUNCTION(BlueprintPure, Exec)
	bool UseHairScreen() const;

	UFUNCTION(BlueprintPure, Exec)
	bool UseWideScreen() const;

protected:
	/** 填充Widget，并且必须是CanvasPanelSlot */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResizeStableFillWidgets();

#if UE_VERSION_OLDER_THAN(5, 1, 0)
public:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override
	{
	}
#endif
};
