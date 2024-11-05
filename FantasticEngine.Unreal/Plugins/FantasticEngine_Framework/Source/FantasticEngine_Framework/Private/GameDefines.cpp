// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDefines.h"

#include "FEGameInstance.h"
#include "GameRoot.h"
#include "Defines/DataLanguage.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/InputSystem.h"

TSubclassOf<UAudioClassConfigs> UGameDefines::GetAudioClassConfigs() const
{
	return AudioClassConfigs;
}

EMouseType UGameDefines::GetMouseType() const
{
	return MouseType;
}

void UGameDefines::SetMouseType(EMouseType NewMouseType, const bool bChangeCache)
{
	if (MouseType == EMouseType::Default)
	{
		MouseType = EMouseType::Visible;
	}
	if (NewMouseType == EMouseType::Default)
	{
		UInputSystem::SetCursorVisible(this, MouseType == EMouseType::Visible);
		return;
	}
	if (bChangeCache)
	{
		MouseType = NewMouseType;
	}
	UInputSystem::SetCursorVisible(this, NewMouseType == EMouseType::Visible);
}

EScreenAdapter UGameDefines::GetScreenAdapter() const
{
	return ScreenAdapter;
}

bool UGameDefines::UseHairScreenAdapter() const
{
	return ScreenAdapter == EScreenAdapter::Auto || ScreenAdapter == EScreenAdapter::HairScreen;
}

bool UGameDefines::UseWideScreenAdapter() const
{
	return ScreenAdapter == EScreenAdapter::Auto || ScreenAdapter == EScreenAdapter::WideScreen;
}

const UDataTable* UGameDefines::GetTableLanguage() const
{
	return TableLanguage.Get();
}

FText UGameDefines::GetLanguage(int32 LanguageId) const
{
	if (TableLanguage)
	{
		FString ContextString;
		FName Name = FName(FString::FromInt(LanguageId));
		FDataLanguage* Language = TableLanguage->FindRow<FDataLanguage>(Name, ContextString);
		if (Language != nullptr)
		{
			return Language->Text;
		}
	}
	return FText();
}

ETableLoadMode UGameDefines::GetTableLoadMode() const
{
	return TableLoadMode;
}

FString UGameDefines::GetTableIdxUrl() const
{
	return TableIdxUrl;
}

FString UGameDefines::GetTableUrl() const
{
	return TableUrl;
}

int32 UGameDefines::GetTableShift() const
{
	return TableShift;
}

// TSubclassOf<UGlobalConfig> UFrameworkDefines::GetGlobalConfigClass() const
// {
// 	return GlobalConfigClass;
// }

TSubclassOf<UTipWidgets> UGameDefines::GetTipWidgetsClass() const
{
	return TipWidgetsClass;
}

UMaterialParameterCollection* UGameDefines::GetMPC_Framework() const
{
	return MPC_Framework;
}

void UGameDefines::SetPlayMode(EPlayMode NewValue)
{
	PlayMode = NewValue;
}

bool UGameDefines::IsDebug() const
{
	if (PlayMode == EPlayMode::AUTO)
	{
		return IsEditor();
	}
	return PlayMode == EPlayMode::DEBUG;
}

// void UGameDefines::SetIsEditor(bool NewValue)
// {
// 	bIsEditor = NewValue;
// }

bool UGameDefines::IsEditor()
{
	return GIsEditor;
}

int UGameDefines::GetTipWidgetsZOrder() const
{
	return TipWidgetsZOrder;
}

int UGameDefines::GetDefaultZOrder() const
{
	return DefaultZOrder;
}

UBlueprintBridgeUtilsBase* UGameDefines::GetBlueprintBridgeUtils(TSubclassOf<UBlueprintBridgeUtilsBase> Class) const
{
	return BlueprintBridgeUtils;
}

void UGameDefines::GetConsoleCommands(TArray<FGameDefineCommand>& Commands) const
{
	Commands = ConsoleCommands;
}

UDataTable* UGameDefines::GetSystemConfigs() const
{
	return DT_SystemConfigs;
}

UDataTable* UGameDefines::GetWidgetConfigs() const
{
	return DT_WidgetConfigs;
}

UDataTable* UGameDefines::GetWidgetFilterConfigs() const
{
	return DT_WidgetFilterConfigs;
}

UDataTable* UGameDefines::GetLevelFilterConfigs() const
{
	return DT_LevelFilterConfigs;
}

EPlatform UGameDefines::GetPlatform() const
{
	return Platform;
}

void UGameDefines::Initialize()
{
	Platform = AnalysisPlatform();
	K2_OnInitialize();
#if WITH_EDITOR
	FInternationalization::Get().GetCurrentLanguage();
#endif
}

EPlatform UGameDefines::AnalysisPlatform()
{
	// const FString& PlatformName = FPlatformProperties::PlatformName();
	// if (PlatformName == TEXT("Windows")
	// 	|| PlatformName == TEXT("WindowsServer")
	// 	|| PlatformName == TEXT("WindowsEditor")
	// 	|| PlatformName == TEXT("WindowsClient")
	//
	// 	|| PlatformName == TEXT("Mac")
	// 	|| PlatformName == TEXT("MacServer")
	// 	|| PlatformName == TEXT("MacEditor")
	// 	|| PlatformName == TEXT("MacClient")
	//
	// 	|| PlatformName == TEXT("LinuxArm64")
	// 	|| PlatformName == TEXT("Linux")
	// 	|| PlatformName == TEXT("LinuxArm64Server")
	// 	|| PlatformName == TEXT("LinuxServer")
	// 	|| PlatformName == TEXT("LinuxEditor")
	// 	|| PlatformName == TEXT("LinuxArm64Client")
	// 	|| PlatformName == TEXT("LinuxClient"))
	// {
	// 	return Desktop;
	// }
	// if (PlatformName == TEXT("Android")
	// 	|| PlatformName == TEXT("IOS"))
	// {
	// 	return Mobile;
	// }
	// if (PlatformName == TEXT("HoloLens"))
	// {
	// 	return HoloLens;
	// }
	// return GameConsole;

#if PLATFORM_HOLOLENS
	{
		return EPlatform::HoloLens;
	}
#elif (PLATFORM_SWITCH || PLATFORM_FREEBSD || PLATFORM_TVOS || PLATFORM_UNIX)
	{
		return EPlatform::GameConsole;
	}
#elif (PLATFORM_IOS || PLATFORM_ANDROID)
	{
		return EPlatform::Mobile;
	}
#elif (PLATFORM_MAC || PLATFORM_WINDOWS || PLATFORM_DESKTOP || PLATFORM_LINUX)
	{
		return EPlatform::Desktop;
	}
#else
	return EPlatform::GameConsole;
#endif
}
