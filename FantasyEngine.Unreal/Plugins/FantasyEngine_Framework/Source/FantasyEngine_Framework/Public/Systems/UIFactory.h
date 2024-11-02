// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/AnimConfig.h"
#include "Basis/SystemBasis.h"
#include "Basis/Tips/TipWidgets.h"
#include "Engine/DataTable.h"
#include "UIFactory.generated.h"

class UBaseWidget;

UENUM(BlueprintType)
enum class EUILevel : uint8
{
	/**
	 * 放在所有UI的最底层，并且往下叠加
	 */
	BottomAll = 0,
	/**
	 * 比默认层级低一层级，并且往上叠加
	 */
	Bottom = 1,
	/**
	* 默认层级，并且往上叠加
	*/
	Middle = 2,
	/**
	* 比默认层级高一层级，并且往上叠加
	*/
	Top = 3,
	/**
	* 覆盖于所有UI之上，并且往上叠加
	*/
	TopAll = 4,
};

USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FWidgetConfig : public FTableRowBase
{
	GENERATED_BODY()

public:
	// UPROPERTY(BlueprintReadOnly, EditAnywhere)
	// TEnumAsByte<...> WidgetKeyName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Enable = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<UUserWidget> Widget;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EUILevel UILevel = EUILevel::Middle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Transparent = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimConfig AnimConfig;
};

UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UWidgetParam : public UWorldObject
{
	GENERATED_BODY()
};

UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API UWidgetStackInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName KeyName;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UWidgetParam* Param;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBaseWidget* Widget;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int ZOrder;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bRepaint;
};

USTRUCT()
struct FANTASYENGINE_FRAMEWORK_API FWidgetEvent
{
	GENERATED_BODY()

public:
	bool bToOpen;
	UPROPERTY()
	FName KeyName;
	UPROPERTY()
	UWidgetParam* Param = nullptr;
	bool bInterrupt;
};

// UENUM()
// enum class EWidgetStackEventState:uint8
// {
// 	Waiting=0,
// 	Creating=1,
// 	Executing=2,
// };

USTRUCT()
struct FANTASYENGINE_FRAMEWORK_API FWidgetStackEvent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UWidgetStackInfo* StackInfo = nullptr;
	UPROPERTY()
	bool bOpening;
	UPROPERTY()
	bool bInterrupt;
	UPROPERTY()
	EUIAnimType AnimType;
	UPROPERTY()
	float Seconds;
	UPROPERTY()
	float TotalSeconds;
	UPROPERTY()
	bool bIsCompleted;

	UPROPERTY()
	bool bIsValid = false;

public:
	UBaseWidget* GetWidget() const
	{
		if (StackInfo == nullptr)
		{
			return nullptr;
		}
		return StackInfo->Widget;
	}

	bool IsValid()
	{
		return bIsValid;
	}

	void Reset()
	{
		bIsValid = false;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API UUIFactory : public USystemBasis
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FWorldContext WorldContext;
	UPROPERTY()
	UTipWidgets* Tips;
	UPROPERTY()
	TMap<FName, FWidgetConfig> WidgetConfigs;
	UPROPERTY()
	TArray<UWidgetStackInfo*> WidgetStack;
	UPROPERTY()
	TMap<FName, UBaseWidget*> WidgetCache;

	//TSharedPtr<TQueue<TSharedPtr<FWidgetEvent>>> WidgetEvents = MakeShared<TQueue<TSharedPtr<FWidgetEvent>>>();//TQueue会莫名其妙被回收
	UPROPERTY()
	TArray<FWidgetEvent> WidgetEvents;

	UPROPERTY()
	FWidgetStackEvent CurrentEvent;

	UPROPERTY()
	FVector2D ScreenSize;

public:
	void RegistWidget(const FName WidgetKeyName, const FWidgetConfig Config);

public:
	UFUNCTION(BlueprintPure, Category="UI Factory", meta=(WorldContext="WorldContextObject"))
	static UTipWidgets* GetTips(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="UI Factory")
	static TMap<FName, FWidgetConfig> GetWidgetConfigs(const UObject* WorldContextObject);
	/**
	 * 获取最上层的Widget
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category="UI Factory", meta=(WorldContext="WorldContextObject"))
	static UWidgetStackInfo* GetTopWidget(const UObject* WorldContextObject);
	/**
	 * 最后打开的Widget，当打开和关闭时更新
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category="UI Factory", meta=(WorldContext="WorldContextObject"))
	static UWidgetStackInfo* GetLastOpenedWidget(const UObject* WorldContextObject);

public:
	UFUNCTION()
	void OnWidgetClassLoaded(UClass* WidgetClass);

private:
	int GetValidedZOrder(EUILevel UILevel);

	void BeginPushWidget(const FName& WidgetKeyName, UWidgetParam* Param = nullptr,
	                     bool bInterrupt = false);

	void BeginPushWidget(UBaseWidget* Widget);

	void EndPushWidget(UWidgetStackInfo* StackInfo);

	void BeginPopWidget(const FName& WidgetKeyName, bool bInterrupt = false);

	void EndPopWidget(UWidgetStackInfo* StackInfo);

	void PushStackInfo(UWidgetStackInfo* StackInfo);

	void PopStackInfo(UWidgetStackInfo* StackInfo);

	void ComputeZOrdersAndRepaint();

	void SetVisibility(UWidgetStackInfo* StackInfo, bool bToShow);

	//void OnLevelAdded(ULevel* Level, UWorld* World);

public:
	virtual void OnInitialize_Implementation() override;

	void InitModules();

	virtual void OnTick_Implementation(float DeltaTime) override;

	static void PlayWidgetAnim(UBaseWidget* Widget, EUIAnimType AnimType, bool bOpening, float Alpha);

	virtual void OnReset_Implementation() override;

	virtual void OnQuit_Implementation() override;

public:
	/**
	 * 加载界面
	 * @param WidgetKeyName 界面类型
	 * @param Param 传入参数
	 * @param bInterrupt 如果有正在处理的界面是否需要打断
	 */
	UFUNCTION(BlueprintCallable, Category="UI Factory", meta=(WorldContext="WorldContextObject"))
	static void PushWidget(const UObject* WorldContextObject, FName WidgetKeyName, UWidgetParam* Param = nullptr,
	                       bool bInterrupt = false);

	/**
	 * 移除界面
	 * @param WidgetKeyName 界面类型
	 * @param bInterrupt 如果有正在处理的界面是否需要打断
	 */
	UFUNCTION(BlueprintCallable, Category="UI Factory", meta=(WorldContext="WorldContextObject"))
	static void PopWidget(const UObject* WorldContextObject, FName WidgetKeyName = NAME_None, bool bInterrupt = false);

	/**
	 * 移除所有加载过的界面
	 */
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void PopAllWidgets(const UObject* WorldContextObject);

public:
	/**
	 * 获取屏幕大小
	 */
	UFUNCTION(BlueprintPure, Exec, Category="UI Factory", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetScreenSize(const UObject* WorldContextObject);
	// UFUNCTION(BlueprintCallable)
	// static void RemoveWidgetFromCache(const FString& WidgetKeyName);
};
