#pragma once

#include "CoreMinimal.h"

// #include "Public/Basis/SystemBasis.h"
//
// #include "Public/Systems/EventSystem.h"
//
// #include "Public/Utilities/LogUtility.h"
// #include "Public/Utilities/ObjectFactory.h"
//
// #include "Public/MainClass.h"

class FFantasyEngine_FrameworkModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnPostEngineInit();
};
