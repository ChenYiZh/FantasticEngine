#pragma once

#include "CoreMinimal.h"
#include "FantasyEngineFrameworkAssetTools.h"
#include "Modules/ModuleManager.h"
#include "Toolbar/FantasyEngineToolbar.h"

class FToolBarBuilder;
class FMenuBuilder;

class FFantasyEngine_Framework_EditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnPostEngineInit();

public:
	/** 主工具栏 */
	TSharedPtr<FFantasyEngineToolbar> FantasyEngineToolbar = nullptr;
	/** 右键菜单栏 */
	TSharedPtr<FFantasyEngineFrameworkAssetTools> AssetTools = nullptr;
};
