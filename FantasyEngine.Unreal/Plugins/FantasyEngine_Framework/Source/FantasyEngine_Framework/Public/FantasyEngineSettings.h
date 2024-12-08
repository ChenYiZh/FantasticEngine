// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRoot.h"
#include "UObject/NoExportTypes.h"
#include "FantasyEngineSettings.generated.h"

/**
 * 
 */
UCLASS(config=FantasyEngineSettings, defaultconfig, meta=(DisplayName="Fantasy Engine Settings"))
class FANTASYENGINE_FRAMEWORK_API UFantasyEngineSettings : public UObject
{
	GENERATED_BODY()

public:
	/** 全局类 */
	UPROPERTY(config, NoClear, EditAnywhere, Category="Configs",
		meta=(DisplayThumbnail="true", MetaClass="/Script/FantasyEngine_Framework.GameRoot"))
	FSoftClassPath GameRoot;
	/** 蓝图与C++构建的桥梁 */
	UPROPERTY(config, NoClear, EditAnywhere, Category="Configs",
		meta=(DisplayThumbnail="true", MetaClass="/Script/FantasyEngine_Framework.BlueprintBridgeUtilsBase"))
	FSoftClassPath BlueprintBridgeUtils;
};
