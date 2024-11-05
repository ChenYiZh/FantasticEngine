#pragma once

#include "CoreMinimal.h"
#include "FantasticEngineGameplayAssetTools.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FFantasticEngine_Gameplay_EditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	/** 右键菜单栏 */
	TSharedPtr<FFantasticEngineGameplayAssetTools> AssetTools = nullptr;
};
