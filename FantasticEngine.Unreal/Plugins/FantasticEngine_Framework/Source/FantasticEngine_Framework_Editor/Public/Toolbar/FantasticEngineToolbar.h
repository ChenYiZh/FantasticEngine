#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FFantasticEngineToolbar : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command. */
	TSharedRef<SWidget> CreateMenu();

private:
	void RegisterMenus();

private:
	TSharedPtr<FUICommandList> CommandList;
};
