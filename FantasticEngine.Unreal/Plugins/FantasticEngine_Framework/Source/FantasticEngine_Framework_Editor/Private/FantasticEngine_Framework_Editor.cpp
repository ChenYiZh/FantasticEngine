#include "FantasticEngine_Framework_Editor.h"
#include "CoreMinimal.h"
#include "AssetToolsModule.h"
#include "FantasticEngineEditorSettings.h"
#include "FantasticEngineSettings.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"

void FFantasticEngine_Framework_EditorModule::StartupModule()
{
	//FCoreDelegates::OnPostEngineInit.AddRaw(this, &FFrameworkEditorModule::OnPostEngineInit);


	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(TEXT("Project"),
								 TEXT("Fantastic Engine"),
								 TEXT("Fantastic Engine Settings"),
								 FText::FromString(TEXT("Settings")),
								 FText::FromString(TEXT("Fantastic Engine Settings")),
								 GetMutableDefault<UFantasticEngineSettings>());
		SettingsModule->RegisterSettings(TEXT("Project"),
		                                 TEXT("Fantastic Engine"),
		                                 TEXT("Fantastic Engine Editor"),
		                                 FText::FromString(TEXT("Editor")),
		                                 FText::FromString(TEXT("Fantastic Engine Editor")),
		                                 GetMutableDefault<UFantasticEngineEditorSettings>());
	}
	FantasticEngineToolbar = MakeShareable(new FFantasticEngineToolbar);
	FantasticEngineToolbar->StartupModule();
	AssetTools = MakeShareable(new FFantasticEngineFrameworkAssetTools);
	AssetTools->StartupModule();
}

void FFantasticEngine_Framework_EditorModule::ShutdownModule()
{
	AssetTools->ShutdownModule();
	AssetTools.Reset();
	FantasticEngineToolbar->ShutdownModule();
	FantasticEngineToolbar.Reset();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(TEXT("Project"),
		                                   TEXT("Fantastic Engine"),
		                                   TEXT("Fantastic Engine Editor"));
		SettingsModule->UnregisterSettings(TEXT("Project"),
								   TEXT("Fantastic Engine"),
								   TEXT("Framework"));
	}

	FCoreDelegates::OnPostEngineInit.RemoveAll(this);
}

void FFantasticEngine_Framework_EditorModule::OnPostEngineInit()
{
	FantasticEngineToolbar->StartupModule();
}

IMPLEMENT_GAME_MODULE(FFantasticEngine_Framework_EditorModule, FantasticEngine_Framework_Editor);
