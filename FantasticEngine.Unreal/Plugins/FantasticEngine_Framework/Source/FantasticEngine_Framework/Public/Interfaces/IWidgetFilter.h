// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/AnimConfig.h"
#include "Basis/BaseWidget.h"
#include "Systems/UIFactory.h"
#include "UObject/Interface.h"
#include "IWidgetFilter.generated.h"

USTRUCT(BlueprintType)
struct FANTASTICENGINE_FRAMEWORK_API FWidgetFilterConfig : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	 * 全局UI过滤器类型
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MustImplement="IWidgetFilter"))
	TSubclassOf<UObject> Filter;
	/**
	 * 是否启用
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Enable = true;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIWidgetFilter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FANTASTICENGINE_FRAMEWORK_API IIWidgetFilter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool OnWidgetOpening(const FName& WidgetKeyName,UPARAM(ref) EUIAnimType& AnimationIn,
	                     UPARAM(ref) float& AnimationInSeconds,
	                     UWidgetParam* Param);
	virtual bool OnWidgetOpening_Implementation(const FName& WidgetKeyName, EUIAnimType& AnimationIn,
	                                            float& AnimationInSeconds, UWidgetParam* Param);

	UFUNCTION(BlueprintNativeEvent)
	void OnWidgetOpen(UBaseWidget* Widget, UWidgetParam* Param);
	virtual void OnWidgetOpen_Implementation(UBaseWidget* Widget, UWidgetParam* Param);

	UFUNCTION(BlueprintNativeEvent)
	void OnWidgetShown(UBaseWidget* Widget);
	virtual void OnWidgetShown_Implementation(UBaseWidget* Widget);

	UFUNCTION(BlueprintNativeEvent)
	bool OnWidgetClosing(UBaseWidget* Widget,UPARAM(ref) EUIAnimType& AnimationOut,
	                     UPARAM(ref) float& AnimationOutSeconds);
	virtual bool OnWidgetClosing_Implementation(UBaseWidget* Widget, EUIAnimType& AnimationOut,
	                                            float& AnimationOutSeconds);

	UFUNCTION(BlueprintNativeEvent)
	void OnWidgetHidden(UBaseWidget* Widget);
	virtual void OnWidgetHidden_Implementation(UBaseWidget* Widget);

	UFUNCTION(BlueprintNativeEvent)
	void OnWidgetClosed(UBaseWidget* Widget);
	virtual void OnWidgetClosed_Implementation(UBaseWidget* Widget);
};
