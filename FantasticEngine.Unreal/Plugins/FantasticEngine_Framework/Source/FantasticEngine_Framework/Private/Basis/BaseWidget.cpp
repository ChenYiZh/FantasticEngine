// Fill out your copyright notice in the Description page of Project Settings.


#include "Basis/BaseWidget.h"

#include "GameRoot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Log/FEConsole.h"
#include "Utilities/DeviceUtility.h"
#include "Utilities/GraphicUtility.h"
#if WITH_EDITOR
const FText UMiniWidget::GetPaletteCategory()
{
	return ItemPaletteCategory;
}
#endif
void UMiniWidget::NativeOnCreate()
{
	OnCreate();
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UMiniWidget* MiniWidget = Cast<UMiniWidget>(Widget))
		{
			MiniWidget->NativeOnCreate();
		}
	}
	TranslateUIText();
}


void UMiniWidget::NativeOnWidgetOpen(UWidgetParam* Param)
{
	OnWidgetOpen(Param);
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UMiniWidget* MiniWidget = Cast<UMiniWidget>(Widget))
		{
			MiniWidget->NativeOnWidgetOpen(Param);
		}
	}
	TranslateUIText();
}

void UMiniWidget::NativeOnShow()
{
	OnShow();
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UMiniWidget* MiniWidget = Cast<UMiniWidget>(Widget))
		{
			MiniWidget->NativeOnShow();
		}
	}
}

void UMiniWidget::NativeOnHide()
{
	OnHide();
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UMiniWidget* MiniWidget = Cast<UMiniWidget>(Widget))
		{
			MiniWidget->NativeOnHide();
		}
	}
}

void UMiniWidget::NativeOnWidgetClosed()
{
	OnWidgetClosed();
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UMiniWidget* MiniWidget = Cast<UMiniWidget>(Widget))
		{
			MiniWidget->NativeOnWidgetClosed();
		}
	}
}

void UMiniWidget::NativeOnTick(float DeltaTime)
{
	OnTick(DeltaTime);
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UMiniWidget* MiniWidget = Cast<UMiniWidget>(Widget))
		{
			MiniWidget->NativeOnTick(DeltaTime);
		}
	}
}

void UMiniWidget::NativeOnEverySecond()
{
	OnEverySecond();
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UMiniWidget* MiniWidget = Cast<UMiniWidget>(Widget))
		{
			MiniWidget->NativeOnEverySecond();
		}
	}
}

void UMiniWidget::GetAllWidgets(TArray<UWidget*>& Widgets)
{
	Widgets.Empty();
	if (WidgetTree)
	{
		WidgetTree->GetAllWidgets(Widgets);
	}
}

void UMiniWidget::TranslateUIText_Implementation()
{
}


void UMiniWidget::OnCreate_Implementation()
{
}

void UMiniWidget::OnWidgetOpen_Implementation(UWidgetParam* Param)
{
}

void UMiniWidget::OnShow_Implementation()
{
}

void UMiniWidget::OnHide_Implementation()
{
}

void UMiniWidget::OnWidgetClosed_Implementation()
{
}

void UMiniWidget::OnTick_Implementation(float DeltaTime)
{
}

void UMiniWidget::OnEverySecond_Implementation()
{
}

#if WITH_EDITOR
const FText UBaseWidget::GetPaletteCategory()
{
	return NSLOCTEXT("UI", "UI", "UI");
}
#endif

FName UBaseWidget::GetKeyName() const
{
	return KeyName;
}

void UBaseWidget::SetKeyName(const FName& InKeyName)
{
	if ((KeyName.IsNone() || KeyName == NAME_None || !KeyName.IsValid())
		&& !(InKeyName == NAME_None || InKeyName.IsNone() || !InKeyName.IsValid()))
	{
		KeyName = InKeyName;
	}
	else
	{
		UFEConsole::WriteWarnWithCategory(TEXT("UI Factory"), FString::Printf(TEXT("%s 已经设置过BaseWidgetName： %s"),
		                                                                     ToCStr(GetClass()->GetName()),
		                                                                     ToCStr(KeyName.ToString())));
	}
}

void UBaseWidget::BeginShow_Implementation(EUIAnimType& AnimIn, float& AnimInSeconds)
{
}

void UBaseWidget::CustomAnimInLoop_Implementation(float Alpha)
{
}

void UBaseWidget::BeginHide_Implementation(EUIAnimType& AnimOut, float& AnimOutSeconds)
{
}

void UBaseWidget::CustomAnimOutLoop_Implementation(float Alpha)
{
}

void UBaseWidget::Close_Implementation()
{
	UUIFactory::PopWidget(this, KeyName);
}

void UBaseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	try
	{
		NativeOnTick(InDeltaTime);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Widget"), FString::Printf(TEXT("%s OnTick 报错"), ToCStr(GetKeyName().ToString())));
	}
	Seconds += InDeltaTime;
	if (Seconds >= 1)
	{
		Seconds = 0;
		try
		{
			NativeOnEverySecond();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("Widget"), FString::Printf(TEXT("%s OnEverySecond 报错"), ToCStr(GetKeyName().ToString())));
		}
	}
}

FAnchors UBaseWidget::GetStableAnchors()
{
	const UGameDefines* Defines = UGameRoot::GetGameDefines(this);
	const bool IsMobile = UDeviceUtility::IsMobile(this);
	const FVector2D ScreenSize = UUIFactory::GetScreenSize(this);
	float OffsetX = 0;
	if (Defines->GetScreenAdapter() == EScreenAdapter::Auto)
	{
		if (IsMobile)
		{
			if (UseHairScreen() && UGraphicUtility::IsHairScreen(this))
			{
				OffsetX = Defines->HairScreenOffset / ScreenSize.X;
			}
		}
		else
		{
			if (UseWideScreen() && UGraphicUtility::IsWideScreen(this))
			{
				OffsetX = (ScreenSize.X - Defines->MaxWideScreenResolution.Y / ScreenSize.Y
						* Defines->MaxWideScreenResolution.X)
					/ ScreenSize.X;
				OffsetX /= 2.f;
			}
		}
	}
	else if (UseHairScreen())
	{
		if (UGraphicUtility::IsHairScreen(this))
		{
			OffsetX = Defines->HairScreenOffset / ScreenSize.X;
		}
	}
	else if (UseWideScreen())
	{
		if (UGraphicUtility::IsWideScreen(this))
		{
			OffsetX = (ScreenSize.X - Defines->MaxWideScreenResolution.Y / ScreenSize.Y
					* Defines->MaxWideScreenResolution.X)
				/ ScreenSize.X;
			OffsetX /= 2.f;
		}
	}
	return FAnchors(OffsetX, 0, 1.f - OffsetX, 1);;
}

bool UBaseWidget::UseHairScreen() const
{
	const UGameDefines* Defines = UGameRoot::GetGameDefines(this);
	return (ScreenAdapter == EScreenAdapter::Auto || ScreenAdapter == EScreenAdapter::HairScreen) && Defines->
		UseHairScreenAdapter();
}

bool UBaseWidget::UseWideScreen() const
{
	const UGameDefines* Defines = UGameRoot::GetGameDefines(this);
	return (ScreenAdapter == EScreenAdapter::Auto || ScreenAdapter == EScreenAdapter::WideScreen) && Defines->
		UseWideScreenAdapter();
}

void UBaseWidget::ResizeStableFillWidgets_Implementation()
{
	TArray<UWidget*> Widgets;
	GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (!Widget->GetName().Contains(TEXT("_Fill")))
		{
			continue;
		}
		if (UCanvasPanelSlot* WidgetSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget))
		{
			WidgetSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
			WidgetSlot->SetAlignment(FVector2D::UnitVector * 0.5f);
			const FVector2D NewSize = UGraphicUtility::FillScreenWithOriginRate(this, WidgetSlot->GetSize());
			WidgetSlot->SetOffsets(FMargin(0, 0, NewSize.X, NewSize.Y));
		}
	}
}

void UBaseWidget::Resize_Implementation()
{
	this->SetAnchorsInViewport(GetStableAnchors());
	ResizeStableFillWidgets();
}
