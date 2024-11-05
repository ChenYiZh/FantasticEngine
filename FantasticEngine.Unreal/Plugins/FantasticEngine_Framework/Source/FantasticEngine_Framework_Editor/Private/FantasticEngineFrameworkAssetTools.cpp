#include "FantasticEngineFrameworkAssetTools.h"

#include "AssetTypeActions_BlueprintThumbnail.h"
#include "IAssetTools.h"
#include "Factory/GameRootFactory.h"

void FFantasticEngineFrameworkAssetTools::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
#if 0
	GameRootObjectAction = MakeShareable(new FAssetTypeActions_GameRoot);
	GameRootObjectAction->Category
		= AssetTools.RegisterAdvancedAssetCategory(
			TEXT("Fantastic Engine"),
			FText::FromString(TEXT("Fantastic Engine")
			));
	AssetTools.RegisterAssetTypeActions(GameRootObjectAction.ToSharedRef());
#endif
	ThumbnailAction = MakeShareable(new FAssetTypeActions_BlueprintThumbnail);
	AssetTools.RegisterAssetTypeActions(ThumbnailAction.ToSharedRef());
}

void FFantasticEngineFrameworkAssetTools::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
#if 0
		AssetTools.UnregisterAssetTypeActions(GameRootObjectAction.ToSharedRef());
#endif
		AssetTools.UnregisterAssetTypeActions(ThumbnailAction.ToSharedRef());
	}
}
