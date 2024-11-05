#include "FantasticEngine_Gameplay_Editor.h"
#include "CoreMinimal.h"
#include "AssetToolsModule.h"
#include "ISettingsModule.h"
#include "FantasticEngineGameplayAssetTools.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FFantasticEngine_Gameplay_EditorModule, FantasticEngine_Gameplay_Editor);

void FFantasticEngine_Gameplay_EditorModule::StartupModule()
{
	AssetTools = MakeShareable(new FFantasticEngineGameplayAssetTools);
	AssetTools->StartupModule();
}

void FFantasticEngine_Gameplay_EditorModule::ShutdownModule()
{
	AssetTools->ShutdownModule();
	AssetTools.Reset();
}
