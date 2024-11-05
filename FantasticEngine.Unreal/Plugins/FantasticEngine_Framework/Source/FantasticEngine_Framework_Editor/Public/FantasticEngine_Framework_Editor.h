#pragma once

#include "CoreMinimal.h"
#include "FantasticEngineFrameworkAssetTools.h"
#include "Modules/ModuleManager.h"
#include "Toolbar/FantasticEngineToolbar.h"

class FToolBarBuilder;
class FMenuBuilder;

class FFantasticEngine_Framework_EditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnPostEngineInit();

public:
	/** 主工具栏 */
	TSharedPtr<FFantasticEngineToolbar> FantasticEngineToolbar = nullptr;
	/** 右键菜单栏 */
	TSharedPtr<FFantasticEngineFrameworkAssetTools> AssetTools = nullptr;
};
