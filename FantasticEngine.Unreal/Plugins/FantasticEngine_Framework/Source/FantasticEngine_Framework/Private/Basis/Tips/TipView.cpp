// Fill out your copyright notice in the Description page of Project Settings.


#include "Basis/Tips/TipView.h"

#include "Basis/Tips/TipWidgets.h"
#include "Log/FEConsole.h"

FString UTipView::GetKeyName()
{
	return KeyName;
}

void UTipView::SetKeyName(const FString& InKeyName)
{
	if (KeyName.IsEmpty() && !InKeyName.IsEmpty())
	{
		KeyName = InKeyName;
	}
	else
	{
		UFEConsole::WriteWarnWithCategory(TEXT("UI Factory"), FString::Printf(TEXT("%s 已经设置过TipViewName： %s"),
		                                                                     ToCStr(GetClass()->GetName()),
		                                                                     ToCStr(KeyName)));
	}
}

void UTipView::OnShow_Implementation(UTipParam* Param)
{
}

void UTipView::OnHide_Implementation()
{
}

void UTipView::OnInitialize_Implementation()
{
}

void UTipView::ShowBegin_Implementation(EUIAnimType& AnimIn, float& AnimInSeconds)
{
}

void UTipView::ShowEnd_Implementation()
{
}

void UTipView::CustomAnimInLoop_Implementation(float Alpha)
{
}

void UTipView::HideBegin_Implementation(EUIAnimType& AnimOut, float& AnimOutSeconds)
{
}

void UTipView::CustomAnimOutLoop_Implementation(float Alpha)
{
}

void UTipView::OnQuit_Implementation()
{
}

void UTipView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	try
	{
		OnTick(InDeltaTime);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Tips"), FString::Printf(TEXT("%s OnTick 报错"), ToCStr(GetKeyName())));
	}
	Seconds += InDeltaTime;
	if (Seconds >= 1)
	{
		Seconds = 0;
		try
		{
			OnEverySecond();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("Tips"), FString::Printf(TEXT("%s OnEverySecond 报错"), ToCStr(GetKeyName())));
		}
	}
}

#if WITH_EDITOR
const FText UTipView::GetPaletteCategory()
{
	return NSLOCTEXT("UI", "Tip View", "Tip View");
}
#endif

void UTipView::OnEverySecond_Implementation()
{
}

void UTipView::OnTick_Implementation(float DeltaTime)
{
}
