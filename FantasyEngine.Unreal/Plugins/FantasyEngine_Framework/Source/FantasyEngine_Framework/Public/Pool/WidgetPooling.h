// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelWidget.h"
#include "Pool/ObjectPooling.h"
#include "WidgetPooling.generated.h"

/**
 * UI对象池
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UWidgetPooling final : public UObjectPooling
{
	GENERATED_BODY()

private:
	/**
	 * 父节点
	 */
	UPROPERTY()
	UPanelWidget* ParentWidget;
	/**
	 * 默认显示状态
	 */
	UPROPERTY()
	ESlateVisibility VisibilityType = ESlateVisibility::Visible;

public:
	/**
	 * 初始化函数，代替Initialize
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeWidgetPool(TSubclassOf<UUserWidget> WidgetItem, UPanelWidget* PanelWidget,
	                          ESlateVisibility Visibility = ESlateVisibility::Visible);
	/**
	 * 设置父节点
	 */
	UFUNCTION(BlueprintCallable)
	void SetParent(UPanelWidget* PanelWidget);
	/**
	 * 设置默认显示状态
	 */
	UFUNCTION(BlueprintCallable)
	void SetVisibilityType(ESlateVisibility Visibility = ESlateVisibility::Visible);
	/**
	 * 获取所有正在使用的Widget
	 */
	UFUNCTION(BlueprintCallable, Category="Object Pool",
		meta=(DeterminesOutputType="InWidgetClass", DynamicOutputParam="OutWidgets"))
	void GetUsingWidgets(TSubclassOf<UWidget> InWidgetClass, TArray<UWidget*>& OutWidgets) const;
protected:
	/**
	 * 重写创造函数
	 */
	virtual UObject* CreateObject() override;
	/**
	 * 重写使用处理
	 */
	virtual void BeforeUse(UPoolItemHandle* Handle) override;
	/**
	 * 重写使用处理
	 */
	virtual void OnUse(UPoolItemHandle* Handle) override;
	/**
	 * 重写回收处理
	 */
	virtual void OnCollect(UPoolItemHandle* Handle) override;
};
