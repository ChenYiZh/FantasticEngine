#include "FantasyEngine_Gameplay_Editor.h"
#include "CoreMinimal.h"
#include "AssetToolsModule.h"
#include "ISettingsModule.h"
#include "FantasyEngineGameplayAssetTools.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FFantasyEngine_Gameplay_EditorModule, FantasyEngine_Gameplay_Editor);

void FFantasyEngine_Gameplay_EditorModule::StartupModule()
{
	AssetTools = MakeShareable(new FFantasyEngineGameplayAssetTools);
	AssetTools->StartupModule();
}

void FFantasyEngine_Gameplay_EditorModule::ShutdownModule()
{
	AssetTools->ShutdownModule();
	AssetTools.Reset();
}
