#include "FantasticEngineGameplayAssetTools.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_GameplayBlueprintThumbnail.h"
#include "FantasticEngineGameplayStyle.h"
#include "IAssetTools.h"

void FFantasticEngineGameplayAssetTools::StartupModule()
{
	FFantasticEngineGameplayStyle::Initialize();
	FFantasticEngineGameplayStyle::ReloadTextures();
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	// ThumbnailAction = MakeShareable(new FAssetTypeActions_GameplayBlueprintThumbnail);
	// AssetTools.RegisterAssetTypeActions(ThumbnailAction.ToSharedRef());
}

void FFantasticEngineGameplayAssetTools::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.UnregisterAssetTypeActions(ThumbnailAction.ToSharedRef());
	}
	FFantasticEngineGameplayStyle::Shutdown();
}
