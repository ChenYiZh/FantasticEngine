#pragma once
#include "Modules/ModuleInterface.h"

/** UFactory注册 */
class FFantasyEngineGameplayAssetTools : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<class FAssetTypeActions_GameplayBlueprintThumbnail> ThumbnailAction;
};
