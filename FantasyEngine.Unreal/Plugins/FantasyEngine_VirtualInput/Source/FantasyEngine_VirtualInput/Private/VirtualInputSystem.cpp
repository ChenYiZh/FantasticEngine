// Fill out your copyright notice in the Description page of Project Settings.


#include "VirtualInputSystem.h"

#include "GameRoot.h"
#include "Utilities/ObjectFactory.h"
#include "Core/InputDragHandle.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UInputDragHandle* UVirtualInputSystem::GetHandle() const
{
	return Handle;
}

TSet<UVirtualJoystickWidget*> UVirtualInputSystem::GetWidgets()
{
	return Widgets;
}

void UVirtualInputSystem::InitInputSystem(const FVirtualInputConfig& InConfig)
{
	Config = InConfig;
}

void UVirtualInputSystem::CreateVirtualJoysticks()
{
	if (Handle == nullptr)
	{
		Handle = CreateWidget<UInputDragHandle>(UGameRoot::GetRoot(GetWorld())->GetWorld());
		//Handle->AddToRoot();
		Handle->AddToViewport(Config.HandleZOrder);
		Handle->LeftVerticalArea = Config.VerticalArea;
	}
}

void UVirtualInputSystem::RegistVirtualJoystickWidget(UVirtualJoystickWidget* VirtualJoystickWidget)
{
	if (VirtualJoystickWidget != nullptr && !Widgets.Contains(VirtualJoystickWidget))
	{
		Widgets.Add(VirtualJoystickWidget);
		if (Handle != nullptr)
		{
			Handle->OnTouchDown.AddUFunction(VirtualJoystickWidget, FName("OnTouchDownDelegate"));
			Handle->OnTouchMove.AddUFunction(VirtualJoystickWidget, FName("OnTouchMoveDelegate"));
			Handle->OnTouchUp.AddUFunction(VirtualJoystickWidget, FName("OnTouchUpDelegate"));
		}
	}
}

// void UInputSystem::CreateVirtualJoysticksWithDisplay(TSubclassOf<UVirtualJoystickDisplay> VirtualJoystickDisplayClass)
// {
// 	CreateVirtualJoysticks();
// 	AddVirtualJoysticksDisplay(VirtualJoystickDisplayClass);
// }
//
// void UInputSystem::AddVirtualJoysticksDisplay(TSubclassOf<UVirtualJoystickDisplay> VirtualJoystickDisplayClass)
// {
// 	if (VirtualJoystickDisplayClass != nullptr)
// 	{
// 		UVirtualJoystickDisplay* Display = CreateWidget<UVirtualJoystickDisplay>(
// 			UWorldUtility::FindGameWorld(), VirtualJoystickDisplayClass.Get());
// 		if (Display != nullptr)
// 		{
// 			Display->AddToViewport(Config.DefaultDisplayZOrder + 1 + Displays.Num());
// 			Displays.Add(Display);
// 			if (Handle != nullptr)
// 			{
// 				Handle->OnTouchDown.AddUFunction(Display, FName("OnTouchDownDelegate"));
// 				Handle->OnTouchMove.AddUFunction(Display, FName("OnTouchMoveDelegate"));
// 				Handle->OnTouchUp.AddUFunction(Display, FName("OnTouchUpDelegate"));
// 			}
// 		}
// 	}
// }

void UVirtualInputSystem::RemoveAll()
{
	if (Handle != nullptr)
	{
		Handle->RemoveFromParent();
		//Destroy(Handle);
	}
	Handle = nullptr;
	for (UVirtualJoystickWidget* Widget : Widgets)
	{
		if (Widget != nullptr)
		{
			Widget->RemoveFromParent();
			//UUIFactory::RemoveWidgetFromCache(Widget->GetKeyName());
		}
	}
	Widgets.Empty();
}

// void UInputSystem::SetVirtualJoysticksVisible(bool Visible)
// {
// 	if (Handle != nullptr)
// 	{
// 		Handle->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
// 	}
// 	for (int i = Widgets.Num() - 1; i >= 0; i--)
// 	{
// 		auto Display = Widgets[i];
// 		if (Display != nullptr)
// 		{
// 			Display->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
// 		}
// 		else
// 		{
// 			Widgets.RemoveAt(i);
// 		}
// 	}
// }
