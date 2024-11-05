// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/DeviceUtility.h"

#include "GameRoot.h"

bool UDeviceUtility::IsMobile(const UObject* WorldContextObject)
{
	// FString Platform = FPlatformProperties::PlatformName();
	// if (UMainClass::GetRoot())
	// {
	// 	return Platform == FString("Android") || Platform == FString("IOS")
	// 		|| UMainClass::GetRoot()->GetGameDefines()->Get;
	// }
	// return Platform == FString("Android") || Platform == FString("IOS");
	return GetGameDefines<UGameDefines>(WorldContextObject)->GetPlatform() == EPlatform::Mobile;
}

FString UDeviceUtility::MacAddress()
{
	return FPlatformMisc::GetMacAddressString();
}

FString UDeviceUtility::MachineId()
{
	return FPlatformMisc::GetLoginId();
}
