#pragma once
/** UFactory注册 */
class FFantasyEngineFrameworkAssetTools : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#if 0
	TSharedPtr<class FAssetTypeActions_GameRoot> GameRootObjectAction;
#endif
	TSharedPtr<class FAssetTypeActions_BlueprintThumbnail> ThumbnailAction;
};
