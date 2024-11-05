#pragma once

#include "CoreMinimal.h"

class FFantasticEngine_VirtualInputModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
