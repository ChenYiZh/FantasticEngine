#include "FantasticEngine_Framework.h"

//#include "ContentBrowserModule.h"
//#include "ToolMenus.h"
#include "FantasticEngineSettings.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FFantasticEngine_FrameworkModule, FantasticEngine_Framework);

void FFantasticEngine_FrameworkModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FFantasticEngine_FrameworkModule::OnPostEngineInit);
}

void FFantasticEngine_FrameworkModule::ShutdownModule()
{
	// if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	// {
	// 	SettingsModule->UnregisterSettings(TEXT("Project"),
	// 	                                   TEXT("Fantastic Engine"),
	// 	                                   TEXT("Framework"));
	// }
	//
	// FCoreDelegates::OnPostEngineInit.RemoveAll(this);
}

void FFantasticEngine_FrameworkModule::OnPostEngineInit()
{
	// if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	// {
	// 	SettingsModule->RegisterSettings(TEXT("Project"),
	// 									 TEXT("Fantastic Engine"),
	// 									 TEXT("Framework"),
	// 									 FText::FromString(TEXT("Settings")),
	// 									 FText::FromString(TEXT("Framework Settings")),
	// 									 GetMutableDefault<UFrameworkSettings>());
	// }
}

//void FFramework::SetupEditModule()
//{
//	FContentBrowserModule& contentBroModule = FModuleManager::LoadModuleChecked<
//		FContentBrowserModule>("ContentBrowser");
//	contentBroModule.GetAllPathViewContextMenuExtenders().Add(
//		FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FFramework::AddPathExtender));
//}

//TSharedRef<FExtender> FFramework::AddPathExtender(const TArray<FString>& Strings)
//{
//	TSharedPtr<FExtender> tempExtender = MakeShareable(new FExtender());
//	tempExtender->AddMenuExtension(
//		"PathContextBulkOperations", EExtensionHook::First,
//		TSharedPtr<FUICommandList>(),
//		FMenuExtensionDelegate::CreateRaw(this, &FFramework::AddMenuEntry)
//	);
//	return tempExtender.ToSharedRef();
//}

//void FFramework::AddMenuEntry(FMenuBuilder& MenuBuilder)
//{
//	MenuBuilder.AddSubMenu(
//		FText::FromString(TEXT("Framework")),
//		FText::FromString(TEXT("Config")),
//		FNewMenuDelegate::CreateRaw(this, &FFramework::CreateConfig),
//		FUIAction(
//			FExecuteAction(),
//			FCanExecuteAction::CreateLambda([=]()-> bool { return true; })
//		),
//		NAME_None,
//		EUserInterfaceActionType::Button,
//		false,
//		FSlateIcon()
//	);
//}

//void FFramework::CreateConfig(FMenuBuilder& MenuBuilder)
//{
//	
//}
