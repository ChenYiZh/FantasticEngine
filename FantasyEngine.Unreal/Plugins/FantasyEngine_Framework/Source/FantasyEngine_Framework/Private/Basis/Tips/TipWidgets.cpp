// Fill out your copyright notice in the Description page of Project Settings.


#include "Basis/Tips/TipWidgets.h"

#include "GameRoot.h"
#include "Log/FEConsole.h"
#include "Systems/UIFactory.h"
#include "Utilities/GraphicUtility.h"

UTipWidgets* UTipWidgets::GetRoot()
{
	return Root;
}

void UTipWidgets::BeginInitialize()
{
	Super::NativeConstruct();
	Root = this;
	OnInitialize();
	for (TPair<FString, UTipView*>& Pair : Views)
	{
		if (Pair.Value != nullptr && Pair.Value->IsVisible())
		{
			Pair.Value->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	for (TPair<FString, UTipView*>& Pair : Views)
	{
		if (Pair.Value->bInitializeOnBegin)
		{
			Pair.Value->bInitialized = true;
			try
			{
				Pair.Value->OnInitialize();
			}
			catch (...)
			{
				UFEConsole::WriteErrorWithCategory(
					TEXT("Tips"), FString::Printf(TEXT("%s has error OnInitialize."), ToCStr(Pair.Key)));
			}
		}
	}
}

void UTipWidgets::OnInitialize_Implementation()
{
	Registing();
}

void UTipWidgets::Registing_Implementation()
{
}


void UTipWidgets::ShowImpl_Implementation(const FString& TipKeyName, bool bHideOthers, UTipParam* Param)
{
	if (!Views.Contains(TipKeyName))
	{
		UFEConsole::WriteWarnWithCategory(TEXT("UI Factory"), FString::Printf(TEXT("%s Tips 未注册"), ToCStr(TipKeyName)));
		return;
	}
	UTipView* View = Views[TipKeyName];
	if (bHideOthers && View->bHideOthers)
	{
		TArray<FString> ToHideKeys;
		for (TPair<FString, UTipView*>& Pair : Views)
		{
			if (Pair.Key != TipKeyName && !Pair.Value->bIgnoreHideOthers && IsViewVisible(Pair.Value))
			{
				ToHideKeys.Add(Pair.Key);
			}
		}
		for (FString& Key : ToHideKeys)
		{
			HideImpl(Key);
		}
	}

	if (TipActions.Contains(TipKeyName))
	{
		FTipAnimAction& Action = TipActions[TipKeyName];
		TipActions.Remove(TipKeyName);
		PlayTipAnim(Action.TipView, Action.AnimType, Action.bShowing, 1);
		if (Action.bShowing)
		{
			OnShowEnd(Action.TipView);
		}
		else
		{
			OnHideEnd(Action.TipView);
		}
	}

	if (!View->bInitialized)
	{
		View->bInitialized = true;
		try
		{
			View->OnInitialize();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("Tips"), FString::Printf(TEXT("%s has error OnInitialize."), ToCStr(View->GetKeyName())));
		}
	}
	try
	{
		View->OnShow(Param);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("%s has error OnShow."), ToCStr(View->GetKeyName())));
	}
	EUIAnimType AnimType = View->AnimConfig.AnimationIn;
	float TotalSeconds = View->AnimConfig.AnimationInSeconds;
	try
	{
		View->ShowBegin(AnimType, TotalSeconds);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("%s has error ShowBegin."), ToCStr(View->GetKeyName())));
	}
	View->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (AnimType == EUIAnimType::None)
	{
		OnShowEnd(View);
	}
	else
	{
		FTipAnimAction Action;
		Action.bShowing = true;
		Action.Seconds = 0;
		Action.AnimType = AnimType;
		Action.TipView = View;
		Action.TotalSeconds = TotalSeconds;
		TipActions.Add(TipKeyName, Action);
	}
}

void UTipWidgets::OnShowEnd_Implementation(UTipView* TipView)
{
	try
	{
		TipView->ShowEnd();
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("%s has error ShowEnd."), ToCStr(TipView->GetKeyName())));
	}
}

void UTipWidgets::HideImpl_Implementation(const FString& TipKeyName)
{
	if (!Views.Contains(TipKeyName))
	{
		UFEConsole::WriteWarnWithCategory(TEXT("UI Factory"), FString::Printf(TEXT("%s Tips 未注册"), ToCStr(TipKeyName)));
		return;
	}
	UTipView* View = Views[TipKeyName];
	if (!IsViewVisible(View))
	{
		return;
	}
	if (TipActions.Contains(TipKeyName))
	{
		FTipAnimAction& Action = TipActions[TipKeyName];
		TipActions.Remove(TipKeyName);
		PlayTipAnim(Action.TipView, Action.AnimType, Action.bShowing, 1);
		if (Action.bShowing)
		{
			OnShowEnd(Action.TipView);
		}
		else
		{
			OnHideEnd(Action.TipView);
		}
	}
	EUIAnimType AnimType = View->AnimConfig.AnimationIn;
	float TotalSeconds = View->AnimConfig.AnimationInSeconds;
	try
	{
		View->HideBegin(AnimType, TotalSeconds);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("%s has error HideBegin."), ToCStr(View->GetKeyName())));
	}
	if (AnimType == EUIAnimType::None)
	{
		OnHideEnd(View);
	}
	else
	{
		FTipAnimAction Action;
		Action.bShowing = false;
		Action.Seconds = 0;
		Action.AnimType = AnimType;
		Action.TipView = View;
		Action.TotalSeconds = TotalSeconds;
		TipActions.Add(TipKeyName, Action);
	}
}

void UTipWidgets::OnHideEnd_Implementation(UTipView* TipView)
{
	try
	{
		TipView->OnHide();
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("%s has error OnHide."), ToCStr(TipView->GetKeyName())));
	}
	TipView->SetVisibility(ESlateVisibility::Collapsed);
}

bool UTipWidgets::IsViewVisible(UTipView* View)
{
	return View->IsVisible() && (!TipActions.Contains(View->GetKeyName()) || TipActions[View->GetKeyName()].bShowing);
}

void UTipWidgets::Show(const UObject* WorldContextObject, const FString& TipKeyName, UTipParam* Param,
                       const bool& bHideOthers)
{
	UTipWidgets* Tips = UUIFactory::GetTips(WorldContextObject);
	Tips->ShowImpl(TipKeyName, bHideOthers, Param);
}

void UTipWidgets::Hide(const UObject* WorldContextObject, const FString& TipKeyName)
{
	UTipWidgets* Tips = UUIFactory::GetTips(WorldContextObject);
	Tips->HideImpl(TipKeyName);
}

void UTipWidgets::HideAllTips(const UObject* WorldContextObject)
{
	UTipWidgets* Tips = UUIFactory::GetTips(WorldContextObject);
	for (TPair<FString, UTipView*>& Pair : Tips->Views)
	{
		if (Tips->IsViewVisible(Pair.Value))
		{
			Tips->HideImpl(Pair.Key);
		}
	}
	TArray<FString> Keys;
	Tips->TipActions.GetKeys(Keys);
	for (FString& Key : Keys)
	{
		FTipAnimAction& Action = Tips->TipActions[Key];
		Tips->TipActions.Remove(Key);
		Tips->OnHideEnd(Action.TipView);
	}
}

void UTipWidgets::OnTick_Implementation(float DeltaTime)
{
	if (!TipActions.IsEmpty())
	{
		TArray<FString> Removable;
		for (TPair<FString, FTipAnimAction>& Action : TipActions)
		{
			Action.Value.Seconds += DeltaTime;
			float Alpha = FMath::Clamp(Action.Value.Seconds / Action.Value.TotalSeconds, 0.f, 1.f);
			PlayTipAnim(Action.Value.TipView, Action.Value.AnimType, Action.Value.bShowing, Alpha);
			if (Action.Value.Seconds >= Action.Value.TotalSeconds)
			{
				Removable.Add(Action.Key);
			}
		}
		for (FString& Key : Removable)
		{
			FTipAnimAction& Action = TipActions[Key];
			TipActions.Remove(Key);
			if (Action.bShowing)
			{
				OnShowEnd(Action.TipView);
			}
			else
			{
				OnHideEnd(Action.TipView);
			}
		}
	}
}

void UTipWidgets::PlayTipAnim(UTipView* View, EUIAnimType AnimType, bool bOpening, float Alpha)
{
	if (UGameRoot::GetState(this) == EGameRootState::Shutdown)
	{
		return;
	}
	if (AnimType == EUIAnimType::Custom)
	{
		if (bOpening)
		{
			View->CustomAnimInLoop(Alpha);
		}
		else
		{
			View->CustomAnimOutLoop(Alpha);
		}
	}
	else if (AnimType == EUIAnimType::Round)
	{
		float EndMin = 0.f;
		float EndMax = 1.f;
		// if (UGraphicUtility::IsHairScreen() && UMainClass::GetGameDefines()->bAdaptHairScreen
		// 	&& View->bAdapterHairScreen)
		// {
		// 	float Offset = UGraphicUtility::GetHairScreenOffsetRate();
		// 	EndMin = Offset;
		// 	EndMax = 1.f - Offset;
		// }
		float Min = FMath::Lerp(-1.f, EndMin, !bOpening ? 1 - Alpha : Alpha);
		float Max = FMath::Lerp(2.f, EndMax, !bOpening ? 1 - Alpha : Alpha);
		View->SetAnchorsInViewport(FAnchors(Min, Min, Max, Max));
	}
	else
	{
		UGraphicUtility::PlayUIAnim(View, AnimType, !bOpening, Alpha);
	}
}

void UTipWidgets::OnQuit_Implementation()
{
	for (TPair<FString, UTipView*> Pair : Views)
	{
		if (Pair.Value->bInitialized)
		{
			try
			{
				Pair.Value->OnQuit();
			}
			catch (...)
			{
				UFEConsole::WriteErrorWithCategory(
					TEXT("Tips"), FString::Printf(TEXT("%s has error OnQuit."), ToCStr(Pair.Key)));
			}
		}
	}
}

void UTipWidgets::Regist(const FString& TipKeyName, UTipView* TipVIew)
{
	if (TipKeyName.IsEmpty())
	{
		UFEConsole::WriteWarnWithCategory(TEXT("Tips"), TEXT("Tips 注册失败，因为传入空的KeyName"));
		return;
	}
	if (Views.Contains(TipKeyName))
	{
		UFEConsole::WriteWarnWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("Tips %s 注册失败，反复注册"), ToCStr(TipKeyName)));
		return;
	}
	if (TipVIew == nullptr)
	{
		UFEConsole::WriteWarnWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("Tips %s 注册失败，传入TipView为空"), ToCStr(TipKeyName)));
		return;
	}
	TipVIew->SetKeyName(TipKeyName);
	Views.Add(TipKeyName, TipVIew);
}

void UTipWidgets::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	try
	{
		OnTick(InDeltaTime);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Tips"), FString::Printf(TEXT("%s OnTick 报错"), ToCStr(GetName())));
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
				TEXT("Tips"), FString::Printf(TEXT("%s OnEverySecond 报错"), ToCStr(GetName())));
		}
	}
}

#if WITH_EDITOR
const FText UTipWidgets::GetPaletteCategory()
{
	return NSLOCTEXT("UI", "UI", "UI");
}
#endif

void UTipWidgets::OnEverySecond_Implementation()
{
}
