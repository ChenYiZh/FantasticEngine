// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/VirtualInputSystemFunc.h"

#include "VirtualInputSystem.h"
#include "Utilities/ObjectFactory.h"

void UVirtualInputSystemFunc::InitInputSystem(const UObject* WorldContextObject, FVirtualInputConfig InConfig)
{
	Singleton<UVirtualInputSystem>(WorldContextObject)->InitInputSystem(InConfig);
}

void UVirtualInputSystemFunc::CreateVirtualJoysticks(const UObject* WorldContextObject)
{
	Singleton<UVirtualInputSystem>(WorldContextObject)->CreateVirtualJoysticks();
}

void UVirtualInputSystemFunc::RegistVirtualJoystickWidget(UVirtualJoystickWidget* VirtualJoystickWidget)
{
	Singleton<UVirtualInputSystem>(VirtualJoystickWidget)->RegistVirtualJoystickWidget(VirtualJoystickWidget);
}

// void UInputSystemFunc::CreateVirtualJoysticksWithDisplay(
// 	TSubclassOf<UVirtualJoystickWidget> VirtualJoystickDisplayClass)
// {
// 	Singleton<UInputSystem>()->CreateVirtualJoysticksWithDisplay(VirtualJoystickDisplayClass);
// }
//
// void UInputSystemFunc::AddVirtualJoysticksDisplay(TSubclassOf<UVirtualJoystickWidget> VirtualJoystickDisplayClass)
// {
// 	Singleton<UInputSystem>()->AddVirtualJoysticksDisplay(VirtualJoystickDisplayClass);
// }

void UVirtualInputSystemFunc::RemoveAll(const UObject* WorldContextObject)
{
	Singleton<UVirtualInputSystem>(WorldContextObject)->RemoveAll();
}

// void UInputSystemFunc::SetVirtualJoysticksVisible(bool Visible)
// {
// 	Singleton<UInputSystem>()->RemoveAll();
// }
