#include "FantasyEngineGameplayAssetTools.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_GameplayBlueprintThumbnail.h"
#include "FantasyEngineGameplayStyle.h"
#include "IAssetTools.h"

void FFantasyEngineGameplayAssetTools::StartupModule()
{
	FFantasyEngineGameplayStyle::Initialize();
	FFantasyEngineGameplayStyle::ReloadTextures();
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	// ThumbnailAction = MakeShareable(new FAssetTypeActions_GameplayBlueprintThumbnail);
	// AssetTools.RegisterAssetTypeActions(ThumbnailAction.ToSharedRef());
}

void FFantasyEngineGameplayAssetTools::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.UnregisterAssetTypeActions(ThumbnailAction.ToSharedRef());
	}
	FFantasyEngineGameplayStyle::Shutdown();
}
