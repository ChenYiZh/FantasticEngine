#include "FantasyEngine_Framework_Editor.h"
#include "CoreMinimal.h"
#include "AssetToolsModule.h"
#include "FantasyEngineEditorSettings.h"
#include "FantasyEngineSettings.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"

void FFantasyEngine_Framework_EditorModule::StartupModule()
{
	//FCoreDelegates::OnPostEngineInit.AddRaw(this, &FFrameworkEditorModule::OnPostEngineInit);


	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(TEXT("Project"),
								 TEXT("Fantasy Engine"),
								 TEXT("Fantasy Engine Settings"),
								 FText::FromString(TEXT("Settings")),
								 FText::FromString(TEXT("Fantasy Engine Settings")),
								 GetMutableDefault<UFantasyEngineSettings>());
		SettingsModule->RegisterSettings(TEXT("Project"),
		                                 TEXT("Fantasy Engine"),
		                                 TEXT("Fantasy Engine Editor"),
		                                 FText::FromString(TEXT("Editor")),
		                                 FText::FromString(TEXT("Fantasy Engine Editor")),
		                                 GetMutableDefault<UFantasyEngineEditorSettings>());
	}
	FantasyEngineToolbar = MakeShareable(new FFantasyEngineToolbar);
	FantasyEngineToolbar->StartupModule();
	AssetTools = MakeShareable(new FFantasyEngineFrameworkAssetTools);
	AssetTools->StartupModule();
}

void FFantasyEngine_Framework_EditorModule::ShutdownModule()
{
	AssetTools->ShutdownModule();
	AssetTools.Reset();
	FantasyEngineToolbar->ShutdownModule();
	FantasyEngineToolbar.Reset();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(TEXT("Project"),
		                                   TEXT("Fantasy Engine"),
		                                   TEXT("Fantasy Engine Editor"));
		SettingsModule->UnregisterSettings(TEXT("Project"),
								   TEXT("Fantasy Engine"),
								   TEXT("Framework"));
	}

	FCoreDelegates::OnPostEngineInit.RemoveAll(this);
}

void FFantasyEngine_Framework_EditorModule::OnPostEngineInit()
{
	FantasyEngineToolbar->StartupModule();
}

IMPLEMENT_GAME_MODULE(FFantasyEngine_Framework_EditorModule, FantasyEngine_Framework_Editor);
