// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/WidgetPooling.h"

#include "Components/CanvasPanelSlot.h"
#include "Log/FEConsole.h"

void UWidgetPooling::InitializeWidgetPool(TSubclassOf<UUserWidget> WidgetItem, UPanelWidget* PanelWidget,
                                          ESlateVisibility Visibility)
{
	Initialize(WidgetItem);
	SetParent(PanelWidget);
	SetVisibilityType(Visibility);
}

void UWidgetPooling::SetParent(UPanelWidget* PanelWidget)
{
	if (PanelWidget == nullptr)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Widget Pool"),TEXT("WidgetPooling.SetParent调用失败，传入值为空"));
	}
	else
	{
		ParentWidget = PanelWidget;
	}
}

void UWidgetPooling::SetVisibilityType(ESlateVisibility Visibility)
{
	VisibilityType = Visibility;
}

void UWidgetPooling::GetUsingWidgets(TSubclassOf<UWidget> InWidgetClass, TArray<UWidget*>& OutWidgets) const
{
	OutWidgets.Empty();
	OutWidgets.SetNum(UsingItems.Num());
	for (int32 i = 0; i < UsingItems.Num(); i++)
	{
		OutWidgets[i] = UsingItems[i]->GetObject<UWidget>();
	}
}

UObject* UWidgetPooling::CreateObject()
{
	if (ParentWidget == nullptr)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Widget Pool"),TEXT("WidgetPooling.创建对象失败。父容器为空"));
		return nullptr;
	}
	return CreateWidget(ParentWidget, ObjectClass.Get());
}

void UWidgetPooling::BeforeUse(UPoolItemHandle* Handle)
{
	Super::BeforeUse(Handle);
	if (UUserWidget* Widget = Cast<UUserWidget>(Handle->GetObject()))
	{
		ParentWidget->AddChild(Widget);
	}
}

void UWidgetPooling::OnUse(UPoolItemHandle* Handle)
{
	Super::OnUse(Handle);
	if (UUserWidget* Widget = Cast<UUserWidget>(Handle->GetObject()))
	{
		Widget->SetVisibility(VisibilityType);
	}
}

void UWidgetPooling::OnCollect(UPoolItemHandle* Handle)
{
	Super::OnCollect(Handle);
	if (UUserWidget* Widget = Cast<UUserWidget>(Handle->GetObject()))
	{
		Widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
