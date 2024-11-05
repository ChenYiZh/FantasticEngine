// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/UIFactory.h"

#include "GameRoot.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Components/OverlaySlot.h"
#include "Defines/EventIdsBasic.h"
#include "Log/FEConsole.h"
#include "Systems/AssetSystem.h"
#include "Systems/FilterStorage.h"
#include "Systems/InputSystem.h"
#include "Utilities/GraphicUtility.h"

void UUIFactory::RegistWidget(const FName WidgetKeyName, const FWidgetConfig Config)
{
	if (WidgetKeyName.IsNone() || !WidgetKeyName.IsValid() || Config.Widget.IsNull())
	{
		return;
	}
	// if (Config.WidgetClassName != WidgetClassName || Config.WidgetKeyName != WidgetKeyName)
	// {
	// 	return;
	// }
	WidgetConfigs.Add(WidgetKeyName, Config);
}

UTipWidgets* UUIFactory::GetTips(const UObject* WorldContextObject)
{
	return Singleton<UUIFactory>(WorldContextObject)->Tips;
}

TMap<FName, FWidgetConfig> UUIFactory::GetWidgetConfigs(const UObject* WorldContextObject)
{
	return Singleton<UUIFactory>(WorldContextObject)->WidgetConfigs;
}

UWidgetStackInfo* UUIFactory::GetTopWidget(const UObject* WorldContextObject)
{
	UUIFactory* Factory = Singleton<UUIFactory>(WorldContextObject);
	if (Factory->WidgetStack.IsEmpty())
	{
		return nullptr;
	}
	else
	{
		UWidgetStackInfo* StackInfo = Factory->WidgetStack[0];
		for (UWidgetStackInfo* Info : Factory->WidgetStack)
		{
			if (Info->ZOrder > StackInfo->ZOrder)
			{
				StackInfo = Info;
			}
		}
		return StackInfo;
	}
}

UWidgetStackInfo* UUIFactory::GetLastOpenedWidget(const UObject* WorldContextObject)
{
	UUIFactory* Factory = Singleton<UUIFactory>(WorldContextObject);
	if (Factory->WidgetStack.IsEmpty())
	{
		return nullptr;
	}
	else
	{
		return Factory->WidgetStack[Factory->WidgetStack.Num() - 1];
	}
}

int UUIFactory::GetValidedZOrder(EUILevel UILevel)
{
	int ZOrder = UGameRoot::GetGameDefines(this)->GetDefaultZOrder();
	if (UILevel == EUILevel::BottomAll)
	{
		return ZOrder;
	}
	for (UWidgetStackInfo* StackInfo : WidgetStack)
	{
		FWidgetConfig config = WidgetConfigs[StackInfo->KeyName];
		if (config.UILevel <= UILevel && StackInfo->ZOrder >= ZOrder)
		{
			ZOrder = StackInfo->ZOrder + 1;
		}
	}
	return ZOrder;
}

void UUIFactory::BeginPushWidget(const FName& WidgetKeyName, UWidgetParam* Param, bool bInterrupt)
{
	if (WidgetKeyName.IsNone() || !WidgetKeyName.IsValid())
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"),TEXT("调用UIFactory::PushWidget错误，因为传入的Widget Key Name为空"));
		return;
	}

	if (!WidgetConfigs.Contains(WidgetKeyName))
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), FString::Printf(
			                                  TEXT("调用UIFactory::PushWidget错误，因为传入的类型：%s 未注册"),
			                                  ToCStr(WidgetKeyName.ToString())));
		return;
	}
	FWidgetConfig Config = WidgetConfigs[WidgetKeyName];
	EUIAnimType AnimType = Config.AnimConfig.AnimationIn;
	float TotalSeconds = Config.AnimConfig.AnimationInSeconds;
	bool bCanOpen = true;
	TArray<UObject*> Filters = UFilterStorage::GetWidgetFilters(this);
	for (UObject* Filter : Filters)
	{
		if (!IIWidgetFilter::Execute_OnWidgetOpening(Filter, WidgetKeyName, AnimType, TotalSeconds, Param))
		{
			bCanOpen = false;
		}
	}
	if (!bCanOpen)
	{
		return;
	}
	FWidgetStackEvent Event;
	Event.bOpening = true;
	Event.bInterrupt = bInterrupt;
	Event.AnimType = AnimType;
	Event.Seconds = 0;
	Event.bIsCompleted = false;
	Event.TotalSeconds = TotalSeconds;
	Event.bIsValid = true;


	UWidgetStackInfo* StackInfo = nullptr;
	if (!GetGameDefines<UGameDefines>(this)->UseFullStack())
	{
		for (UWidgetStackInfo* Info : WidgetStack)
		{
			if (Info->KeyName == WidgetKeyName)
			{
				StackInfo = Info;
				break;
			}
		}
	}

	if (!IsValid(StackInfo))
	{
		StackInfo = NewObject<UWidgetStackInfo>(this);
	}

	StackInfo->KeyName = WidgetKeyName;
	StackInfo->Param = Param;
	StackInfo->bRepaint = true;

	Event.StackInfo = StackInfo;

	CurrentEvent = Event;

	if (WidgetCache.Contains(WidgetKeyName))
	{
		BeginPushWidget(WidgetCache[WidgetKeyName]);
	}
	else
	{
		FAssetClassCallback Callback;
		Callback.BindUFunction(this, "OnWidgetClassLoaded");
		Singleton<UAssetSystem>(this)->LoadWidgetAsync(Config.Widget.ToSoftObjectPath().GetAssetPath().ToString(),
		                                               Callback);
		//UAssetSystem::LoadUClassAsync(Config.WidgetPath, Callback);
	}
}

void UUIFactory::OnWidgetClassLoaded(UClass* WidgetClass)
{
	if (!CurrentEvent.IsValid())
	{
		return;
	}
	if (WidgetClass == nullptr)
	{
		FWidgetStackEvent& StackEvent = CurrentEvent;
		FWidgetConfig& Config = WidgetConfigs[StackEvent.StackInfo->KeyName];
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), FString::Printf(
			                                  TEXT("调用UIFactory::PushWidget错误，未找到传入界面Key：%s 类型：%s 的文件"),
			                                  ToCStr(StackEvent.StackInfo->KeyName.ToString()),
			                                  ToCStr(Config.Widget.ToString())));
		CurrentEvent.Reset();
		return;
	}
	UBaseWidget* Widget = CreateWidget<UBaseWidget>(this->GetWorld(), WidgetClass);
	if (Widget == nullptr)
	{
		FWidgetStackEvent& StackEvent = CurrentEvent;
		FWidgetConfig& Config = WidgetConfigs[StackEvent.StackInfo->KeyName];
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), FString::Printf(
			                                  TEXT("调用UIFactory::PushWidget错误，未找到传入的类型：%s [%s]的文件或者类型不是BaseWidget的子类"),
			                                  ToCStr(StackEvent.StackInfo->KeyName.ToString()),
			                                  ToCStr(Config.Widget.ToString())));
		CurrentEvent.Reset();
		return;
	}
	Widget->SetKeyName(CurrentEvent.StackInfo->KeyName);
	Widget->NativeOnCreate();
	BeginPushWidget(Widget);
}

void UUIFactory::BeginPushWidget(UBaseWidget* Widget)
{
	FWidgetStackEvent& Event = CurrentEvent;
	UWidgetStackInfo* StackInfo = Event.StackInfo;
	FWidgetConfig Config = WidgetConfigs[StackInfo->KeyName];
	bool bContains = WidgetCache.Contains(StackInfo->KeyName);
	StackInfo->Widget = Widget;
	StackInfo->ZOrder = GetValidedZOrder(Config.UILevel);
	PushStackInfo(StackInfo);
	Widget->Resize();
	if (!bContains)
	{
		WidgetCache.Add(StackInfo->KeyName, Widget);
		// try
		// {
		// 	Widget->OnCreate();
		// }
		// catch (const std::exception& e)
		// {
		// 	UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"),e.what());
		// }
	}
	TArray<UObject*> Filters = UFilterStorage::GetWidgetFilters(this);
	try
	{
		for (UObject* Filter : Filters)
		{
			IIWidgetFilter::Execute_OnWidgetOpen(Filter, Widget, StackInfo->Param);
		}
		Widget->NativeOnWidgetOpen(StackInfo->Param);
	}
	catch (const std::exception& e)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
	}
	try
	{
		Widget->BeginShow(Event.AnimType, Event.TotalSeconds);
	}
	catch (const std::exception& e)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
	}
	Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UInputSystem::SetMouseType(this, Widget->MouseType);
	if (Event.AnimType == EUIAnimType::None)
	{
		EndPushWidget(StackInfo);
	}
	else if (Event.bInterrupt)
	{
		PlayWidgetAnim(Widget, Event.AnimType, true, 1);
		EndPushWidget(StackInfo);
	}
	else
	{
		PlayWidgetAnim(Widget, Event.AnimType, true, 0);
	}
}

void UUIFactory::EndPushWidget(UWidgetStackInfo* StackInfo)
{
	try
	{
		StackInfo->Widget->NativeOnShow();
	}
	catch (const std::exception& e)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
	}
	TArray<UObject*> Filters = UFilterStorage::GetWidgetFilters(this);
	for (UObject* Filter : Filters)
	{
		IIWidgetFilter::Execute_OnWidgetShown(Filter, StackInfo->Widget);
	}
	FWidgetConfig Config = WidgetConfigs[StackInfo->KeyName];
	if (!Config.Transparent)
	{
		SetVisibility(StackInfo, false);
	}
	if (CurrentEvent.IsValid())
	{
		CurrentEvent.Reset();
	}
}

void UUIFactory::BeginPopWidget(const FName& WidgetKeyName, bool bInterrupt)
{
	FName KeyName = WidgetKeyName;
	if ((KeyName.IsNone() || !KeyName.IsValid()) && !WidgetStack.IsEmpty())
	{
		UWidgetStackInfo* Info = WidgetStack[WidgetStack.Num() - 1];
		KeyName = Info->KeyName;
	}
	if ((KeyName.IsNone() || !KeyName.IsValid()) || WidgetStack.IsEmpty())
	{
		return;
	}
	bool bExists = false;
	UWidgetStackInfo* StackInfo = WidgetStack[WidgetStack.Num() - 1];
	for (int i = WidgetStack.Num() - 1; i >= 0; i--)
	{
		StackInfo = WidgetStack[i];
		if (StackInfo->KeyName == KeyName)
		{
			bExists = true;
			break;
		}
	}
	if (!bExists)
	{
		return;
	}
	FWidgetConfig Config = WidgetConfigs[KeyName];
	EUIAnimType AnimType = Config.AnimConfig.AnimationOut;
	float TotalSeconds = Config.AnimConfig.AnimationOutSeconds;
	UBaseWidget* Widget = StackInfo->Widget;
	bool bCanClose = true;
	TArray<UObject*> Filters = UFilterStorage::GetWidgetFilters(this);
	for (UObject* Filter : Filters)
	{
		if (!IIWidgetFilter::Execute_OnWidgetClosing(Filter, Widget, AnimType, TotalSeconds))
		{
			bCanClose = false;
		}
	}
	if (!bCanClose)
	{
		return;
	}
	try
	{
		StackInfo->Widget->BeginHide(AnimType, TotalSeconds);
	}
	catch (const std::exception& e)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
	}
	if (!Config.Transparent)
	{
		SetVisibility(StackInfo, true);
	}
	if (AnimType == EUIAnimType::None)
	{
		EndPopWidget(StackInfo);
	}
	else if (bInterrupt)
	{
		PlayWidgetAnim(Widget, AnimType, false, 1);
		EndPopWidget(StackInfo);
	}
	else
	{
		FWidgetStackEvent Event;
		Event.bOpening = false;
		//Event.Widget = Widget;
		Event.AnimType = AnimType;
		Event.Seconds = 0;
		Event.bIsCompleted = false;
		Event.TotalSeconds = TotalSeconds;
		Event.StackInfo = StackInfo;
		Event.bIsValid = true;
		CurrentEvent = Event;
		PlayWidgetAnim(Widget, AnimType, false, 0);
	}
}

void UUIFactory::EndPopWidget(UWidgetStackInfo* StackInfo)
{
	UInputSystem::SetMouseType(this, EMouseType::Default);
	try
	{
		StackInfo->Widget->NativeOnHide();
	}
	catch (const std::exception& e)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
	}
	TArray<UObject*> Filters = UFilterStorage::GetWidgetFilters(this);
	for (UObject* Filter : Filters)
	{
		IIWidgetFilter::Execute_OnWidgetHidden(Filter, StackInfo->Widget);
	}
	try
	{
		StackInfo->Widget->NativeOnWidgetClosed();
	}
	catch (const std::exception& e)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
	}
	for (UObject* Filter : Filters)
	{
		IIWidgetFilter::Execute_OnWidgetClosed(Filter, StackInfo->Widget);
	}
	PopStackInfo(StackInfo);
	if (CurrentEvent.IsValid())
	{
		CurrentEvent.Reset();
	}
}

void UUIFactory::PushStackInfo(UWidgetStackInfo* StackInfo)
{
	FWidgetConfig Config = WidgetConfigs[StackInfo->KeyName];
	for (UWidgetStackInfo* Info : WidgetStack)
	{
		if (Info->ZOrder > StackInfo->ZOrder)
		{
			Info->ZOrder += 1;
			Info->bRepaint = true;
		}
		else if (Info->ZOrder == StackInfo->ZOrder)
		{
			FWidgetConfig InfoConfig = WidgetConfigs[Info->KeyName];
			if (InfoConfig.UILevel > Config.UILevel)
			{
				Info->ZOrder += 1;
				Info->bRepaint = true;
			}
		}
	}
	StackInfo->bRepaint = true;
	WidgetStack.Push(StackInfo);
	ComputeZOrdersAndRepaint();
}

void UUIFactory::PopStackInfo(UWidgetStackInfo* StackInfo)
{
	StackInfo->Widget->SetVisibility(ESlateVisibility::Collapsed);
	WidgetStack.Remove(StackInfo);
	//Destroy(StackInfo);
	ComputeZOrdersAndRepaint();
}

void UUIFactory::ComputeZOrdersAndRepaint()
{
	TArray<TWeakObjectPtr<UWidgetStackInfo>> SortList; // = TArray(WidgetStack);
	for (UWidgetStackInfo* StackInfo : WidgetStack)
	{
		SortList.Push(StackInfo);
	}
	SortList.StableSort([](const TWeakObjectPtr<UWidgetStackInfo> A, const TWeakObjectPtr<UWidgetStackInfo> B)
	{
		return A->ZOrder < B->ZOrder;
	});
	TSet<FName> UsedClasses;
	for (int i = SortList.Num() - 1; i >= 0; i--)
	{
		UWidgetStackInfo* Info = SortList[i].Get();
		int Current = Info->ZOrder;
		int newZOrder = UGameRoot::GetGameDefines(this)->GetDefaultZOrder() + i;
		Info->ZOrder = newZOrder;
		if ((Info->bRepaint || Current != newZOrder) && !UsedClasses.Contains(Info->KeyName))
		{
			UsedClasses.Add(Info->KeyName);
			if (Info->Widget->IsInViewport())
			{
				if (UGameViewportSubsystem* Subsystem = UGameViewportSubsystem::Get(GetWorld()))
				{
					FGameViewportWidgetSlot ViewportSlot = Subsystem->GetWidgetSlot(Info->Widget);
					ViewportSlot.ZOrder = newZOrder;
					Subsystem->SetWidgetSlot(Info->Widget, ViewportSlot);
				}
				else
				{
					Info->Widget->RemoveFromParent();
					Info->Widget->AddToViewport(newZOrder);
				}
			}
			else
			{
				Info->Widget->AddToViewport(newZOrder);
			}
		}
		Info->bRepaint = false;
	}
	SortList.Reset();
}

void UUIFactory::SetVisibility(UWidgetStackInfo* StackInfo, bool bToShow)
{
	int BottomZOrder = UGameRoot::GetGameDefines(this)->GetDefaultZOrder();
	FWidgetConfig StackConfig = WidgetConfigs[StackInfo->KeyName];
	for (UWidgetStackInfo* Info : WidgetStack)
	{
		FWidgetConfig Config = WidgetConfigs[Info->KeyName];
		if (Info->ZOrder < StackInfo->ZOrder && Info->ZOrder > BottomZOrder
			&& Config.UILevel <= StackConfig.UILevel && !Config.Transparent)
		{
			BottomZOrder = Info->ZOrder;
		}
	}
	for (int i = WidgetStack.Num() - 1; i >= 0; i--)
	{
		UWidgetStackInfo* Info = WidgetStack[i];
		if (Info->ZOrder < StackInfo->ZOrder && Info->ZOrder >= BottomZOrder)
		{
			if (Info->Widget == StackInfo->Widget)
			{
				continue;
			}
			if (bToShow)
			{
				Info->Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				try
				{
					StackInfo->Widget->NativeOnShow();
				}
				catch (const std::exception& e)
				{
					UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
				}
				TArray<UObject*> Filters = UFilterStorage::GetWidgetFilters(this);
				for (UObject* Filter : Filters)
				{
					IIWidgetFilter::Execute_OnWidgetShown(Filter, StackInfo->Widget);
				}
			}
			else
			{
				UInputSystem::SetMouseType(this, EMouseType::Default);
				try
				{
					StackInfo->Widget->NativeOnHide();
				}
				catch (const std::exception& e)
				{
					UFEConsole::WriteErrorWithCategory(TEXT("UI Factory"), e.what());
				}
				TArray<UObject*> Filters = UFilterStorage::GetWidgetFilters(this);
				for (UObject* Filter : Filters)
				{
					IIWidgetFilter::Execute_OnWidgetHidden(Filter, StackInfo->Widget);
				}
				Info->Widget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUIFactory::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
	//CurrentEvent.bIsCompleted = true;
	CurrentEvent.Reset();
	InitModules();
}

void UUIFactory::InitModules()
{
	ScreenSize = UGraphicUtility::GetScreenSize(this);
	TSubclassOf<UTipWidgets> TipsClass = UGameRoot::GetGameDefines(this)->GetTipWidgetsClass();
	if (TipsClass)
	{
		if (Tips != nullptr)
		{
			Destroy(Tips);
		}
		Tips = CreateWidget<UTipWidgets>(this->GetWorld(), TipsClass, FName(TEXT("Tips")));
		Tips->BeginInitialize();
		Tips->AddToViewport(UGameRoot::GetGameDefines(this)->GetTipWidgetsZOrder());
	}
}

void UUIFactory::OnTick_Implementation(float DeltaTime)
{
	if (IsEngineExitRequested())
	{
		return;
	}
	Super::OnTick_Implementation(DeltaTime);
	if (!GEngine->GetWorldContextFromWorld(GetWorld()))
	{
		return;
	}
	const FVector2D CurrentSize = UGraphicUtility::GetScreenSize(this);
	const bool bSizeChanged = !ScreenSize.Equals(CurrentSize);
	if (bSizeChanged)
	{
		ScreenSize = CurrentSize;
		for (const UWidgetStackInfo* WidgetInfo : WidgetStack)
		{
			WidgetInfo->Widget->Resize();
		}
	}
	if (CurrentEvent.IsValid()
		&& !CurrentEvent.bIsCompleted)
	{
		FWidgetStackEvent& Event = CurrentEvent;
		if (Event.GetWidget() != nullptr)
		{
			Event.Seconds += DeltaTime;
			float Alpha = FMath::Clamp(Event.Seconds / Event.TotalSeconds, 0.f, 1.f);
			Event.bIsCompleted = Event.Seconds >= Event.TotalSeconds;
			PlayWidgetAnim(Event.GetWidget(), Event.AnimType, Event.bOpening, Alpha);
			if (Event.bIsCompleted)
			{
				if (Event.bOpening)
				{
					EndPushWidget(Event.StackInfo);
				}
				else
				{
					EndPopWidget(Event.StackInfo);
				}
			}
		}
	}
	else if (!WidgetEvents.IsEmpty())
	{
		FWidgetEvent Event = WidgetEvents[0];
		WidgetEvents.RemoveAt(0);
		FWidgetEvent& EventInfo = Event;
		if (EventInfo.bToOpen)
		{
			BeginPushWidget(EventInfo.KeyName, EventInfo.Param, EventInfo.bInterrupt);
		}
		else
		{
			BeginPopWidget(EventInfo.KeyName, EventInfo.bInterrupt);
		}
	}

	if (bSizeChanged)
	{
		UEventSystem::SendEvent(this, UEventIdsBasic::EID_SCREEN_SIZE_CHANGED());
	}
}

void UUIFactory::PlayWidgetAnim(UBaseWidget* Widget, EUIAnimType AnimType, bool bOpening, float Alpha)
{
	if (UGameRoot::GetState(Widget) == EGameRootState::Shutdown)
	{
		return;
	}
	if (AnimType == EUIAnimType::Custom)
	{
		if (bOpening)
		{
			Widget->CustomAnimInLoop(Alpha);
		}
		else
		{
			Widget->CustomAnimOutLoop(Alpha);
		}
	}
	else if (AnimType == EUIAnimType::Round)
	{
		const FAnchors Anchors = Widget->GetStableAnchors();
		const float Rate = !bOpening ? 1 - Alpha : Alpha;
		FVector2D Minimum = FMath::Lerp(-FVector2D::UnitVector, Anchors.Minimum, Rate);
		FVector2D Maximum = FMath::Lerp(2 * FVector2D::UnitVector, Anchors.Maximum, Rate);
		Widget->SetAnchorsInViewport(FAnchors(Minimum.X, Minimum.Y, Maximum.X, Maximum.Y));
	}
	else
	{
		UGraphicUtility::PlayUIAnim(Widget, AnimType, !bOpening, Alpha);
	}
}

void UUIFactory::OnReset_Implementation()
{
	Super::OnReset_Implementation();
	PopAllWidgets(this);
	WidgetStack.Empty();
	for (TPair<FName, UBaseWidget*>& Pair : WidgetCache)
	{
		Pair.Value->RemoveFromParent();
	}
	WidgetCache.Empty();
}

void UUIFactory::OnQuit_Implementation()
{
	Super::OnQuit_Implementation();
	OnReset();
	if (Tips != nullptr)
	{
		Tips->OnQuit();
		Destroy(Tips);
	}
}

void UUIFactory::PushWidget(const UObject* WorldContextObject, FName WidgetKeyName, UWidgetParam* Param,
                            bool bInterrupt)
{
	UUIFactory* Factory = Singleton<UUIFactory>(WorldContextObject);
	if (bInterrupt && !Factory->CurrentEvent.IsValid())
	{
		Factory->BeginPushWidget(WidgetKeyName, Param, bInterrupt);
	}
	else
	{
		FWidgetEvent Event;
		Event.KeyName = WidgetKeyName;
		Event.bToOpen = true;
		Event.Param = Param;
		Event.bInterrupt = bInterrupt;
		Factory->WidgetEvents.Add(Event);
	}
}

void UUIFactory::PopWidget(const UObject* WorldContextObject, FName WidgetKeyName, bool bInterrupt)
{
	UUIFactory* Factory = Singleton<UUIFactory>(WorldContextObject);
	if (bInterrupt && !Factory->CurrentEvent.IsValid())
	{
		Factory->BeginPopWidget(WidgetKeyName, bInterrupt);
	}
	else
	{
		FWidgetEvent Event;
		Event.KeyName = WidgetKeyName;
		Event.bToOpen = false;
		Event.Param = nullptr;
		Event.bInterrupt = bInterrupt;
		Factory->WidgetEvents.Add(Event);
	}
}

void UUIFactory::PopAllWidgets(const UObject* WorldContextObject)
{
	UUIFactory* Factory = Singleton<UUIFactory>(WorldContextObject);
	if (Factory->CurrentEvent.IsValid()
		&& !Factory->CurrentEvent.bIsCompleted
		&& Factory->CurrentEvent.GetWidget() != nullptr)
	{
		FWidgetStackEvent& CurrentInfo = Factory->CurrentEvent;
		CurrentInfo.bIsCompleted = true;
		if (CurrentInfo.AnimType == EUIAnimType::Custom)
		{
			CurrentInfo.GetWidget()->CustomAnimInLoop(1);
		}
		else
		{
			PlayWidgetAnim(CurrentInfo.GetWidget(), CurrentInfo.AnimType, CurrentInfo.bOpening, 1);
		}
		if (CurrentInfo.bIsCompleted)
		{
			if (CurrentInfo.bOpening)
			{
				Factory->EndPushWidget(CurrentInfo.StackInfo);
			}
			else
			{
				Factory->EndPopWidget(CurrentInfo.StackInfo);
			}
		}
	}
	if (UGameRoot::GetState(WorldContextObject) == EGameRootState::Shutdown)
	{
		Factory->CurrentEvent.Reset();
	}
	TArray<UWidgetStackInfo*>& WidgetStack = Factory->WidgetStack;
	while (!WidgetStack.IsEmpty())
	{
		PopWidget(WorldContextObject, NAME_None, true);
	}
}

FVector2D UUIFactory::GetScreenSize(const UObject* WorldContextObject)
{
	return Singleton<UUIFactory>(WorldContextObject)->ScreenSize;
}

// void UUIFactory::RemoveWidgetFromCache(const FString& WidgetKeyName)
// {
// 	UUIFactory* Factory = Singleton<UUIFactory>();
// 	for (int i = Factory->WidgetStack.Num() - 1; i >= 0; i--)
// 	{
// 		UWidgetStackInfo* StackInfo = Factory->WidgetStack[i];
// 		if (StackInfo->KeyName == WidgetKeyName)
// 		{
// 			Factory->WidgetStack.RemoveAt(i);
// 		}
// 	}
// 	if (Factory->WidgetCache.Contains(WidgetKeyName))
// 	{
// 		Factory->WidgetCache.Remove(WidgetKeyName);
// 	}
// }
