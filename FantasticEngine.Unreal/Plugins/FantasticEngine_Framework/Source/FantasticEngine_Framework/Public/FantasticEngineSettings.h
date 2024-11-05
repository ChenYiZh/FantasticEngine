// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRoot.h"
#include "UObject/NoExportTypes.h"
#include "FantasticEngineSettings.generated.h"

/**
 * 
 */
UCLASS(config=FantasticEngineSettings, defaultconfig, meta=(DisplayName="Fantastic Engine Settings"))
class FANTASTICENGINE_FRAMEWORK_API UFantasticEngineSettings : public UObject
{
	GENERATED_BODY()

public:
	/** 全局类 */
	UPROPERTY(config, NoClear, EditAnywhere, Category="Configs",
		meta=(DisplayThumbnail="true", MetaClass="/Script/FantasticEngine_Framework.GameRoot"))
	FSoftClassPath GameRoot;
	/** 蓝图与C++构建的桥梁 */
	UPROPERTY(config, NoClear, EditAnywhere, Category="Configs",
		meta=(DisplayThumbnail="true", MetaClass="/Script/FantasticEngine_Framework.BlueprintBridgeUtilsBase"))
	FSoftClassPath BlueprintBridgeUtils;
};
