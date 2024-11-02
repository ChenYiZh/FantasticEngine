#include "FantasyEngineFrameworkAssetTools.h"

#include "AssetTypeActions_BlueprintThumbnail.h"
#include "IAssetTools.h"
#include "Factory/GameRootFactory.h"

void FFantasyEngineFrameworkAssetTools::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
#if 0
	GameRootObjectAction = MakeShareable(new FAssetTypeActions_GameRoot);
	GameRootObjectAction->Category
		= AssetTools.RegisterAdvancedAssetCategory(
			TEXT("Fantasy Engine"),
			FText::FromString(TEXT("Fantasy Engine")
			));
	AssetTools.RegisterAssetTypeActions(GameRootObjectAction.ToSharedRef());
#endif
	ThumbnailAction = MakeShareable(new FAssetTypeActions_BlueprintThumbnail);
	AssetTools.RegisterAssetTypeActions(ThumbnailAction.ToSharedRef());
}

void FFantasyEngineFrameworkAssetTools::ShutdownModule()
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
