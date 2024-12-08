#pragma once

#include "CoreMinimal.h"
#include "FantasyEngineGameplayAssetTools.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FFantasyEngine_Gameplay_EditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	/** 右键菜单栏 */
	TSharedPtr<FFantasyEngineGameplayAssetTools> AssetTools = nullptr;
};
