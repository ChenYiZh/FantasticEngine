// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/EnumDefines.h"
#include "Systems/LevelSystem.h"
#include "UObject/NoExportTypes.h"
#include "Map.generated.h"

/**
 * 默认场景的基类，类似 LevelActorScript ，但是生命周期比 LevelScript 更早。
 * 场景加载是使用 Travel 的函数，所以同步还是异步是根据 GameMode 的 Use Seamless Travel
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UMap : public UWorldObject
{
	GENERATED_BODY()

	friend class ULevelSystemTask;
	friend class ULevelSystem;

protected:
	/** 场景加载时调用 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMapEntered();
	/** 退出场景时调用 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMapLeft();

#pragma region Variables

public:
	/** 是否使用引用来加载 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config", meta=(ExposeOnSpawn))
	bool bByObjectReference = true;
	/** 场景的名称，包含相对路径。当 bUseStableName = true 才有效。 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config",
		meta=(ExposeOnSpawn, EditCondition="bUseStableName", EditConditionHides))
	TSoftObjectPtr<UWorld> Map;
	/** 场景的名称，包含相对路径。当 bUseStableName = false 才有效， */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config",
		meta=(ExposeOnSpawn, EditCondition="!bUseStableName", EditConditionHides))
	FString MapName;
	/** 场景启动参数，可以参考 https://docs.unrealengine.com/4.27/zh-CN/ProductionPipelines/CommandLineArguments/ */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config",
		meta=(ExposeOnSpawn, EditCondition="!bUseStableName", EditConditionHides))
	FString Options;
	/** 使用 ServerTravel 还是 ClientTravel ，如果是单机使用true */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config",
		meta=(ExposeOnSpawn))
	bool bUseServerTravel = true;

	/** 在这个关卡中的鼠标显示类型 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Config", meta=(ExposeOnSpawn))
	EMouseType MouseType = EMouseType::Default;

private:
	/** 当前场景 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Variables", meta=(AllowPrivateAccess))
	TWeakObjectPtr<UWorld> CurrentMap;

public:
	/** 获取场景名称 */
	UFUNCTION(BlueprintPure, DisplayName="Get Map Name")
	FString GetMapName() const
	{
		if (bByObjectReference)
		{
			return Map.GetLongPackageName();
		}
		return MapName;
	}

public:
	/** 获取当前场景 */
	FORCEINLINE UWorld* GetCurrentMap() const { return CurrentMap.Get(); }

#pragma endregion

public:
	/** 帧事件 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Tick")
	void K2_Tick(float DeltaSeconds);
	/**
	 * 帧事件
	 */
	virtual void Tick(float DeltaSeconds);
};
