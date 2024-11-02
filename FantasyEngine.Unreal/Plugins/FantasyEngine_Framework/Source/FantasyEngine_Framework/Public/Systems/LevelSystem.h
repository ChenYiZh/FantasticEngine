// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LevelSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStreamLevelLoadCallback, const FString&, StreamLevelName,
                                             const bool, bSuccess);

class UMap;

// USTRUCT(BlueprintType)
// struct FANTASYENGINE_FRAMEWORK_API FLevelInfo
// {
// 	GENERATED_BODY()
//
// public:
// 	UPROPERTY(BlueprintReadOnly)
// 	UBaseMap* Scene = nullptr;
// 	UPROPERTY(BlueprintReadWrite)
// 	ULevelParam* Param;
// };

UENUM(BlueprintType)
enum class ELevelSystem_LoadingState : uint8
{
	WAITING = 0 UMETA(DisplayName="Waiting"),
	LOADING = 1 UMETA(DisplayName="Loading From Package"),
	OPENING = 2 UMETA(DisplayName="Opening The Level")
};


/** 场景管理类 */
UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API ULevelSystem : public USystemBasis
{
	GENERATED_BODY()
	friend class ULevelSystemTask;
#pragma region Log

protected:
	static FName LOG_CATEGORY;
#pragma endregion

public:
	virtual void OnInitialize_Implementation() override;
	virtual void OnTick_Implementation(float DeltaTime) override;

#pragma region Functions

public:
	/** 动态加载场景 */
	UFUNCTION(BlueprintCallable, Category="Level System")
	static void LoadMapAsync(UMap* NewMap, bool bShowLoading = true);

private:
	/** 场景加载实现函数 */
	void LoadMapAsyncImpl(UMap* NewMap, bool bShowLoading = true);

private:
	/** 在世加载场景 */
	void BeginLoadLevel(UMap* NewMap);
#pragma endregion

#pragma region Variables

private:
	/** 正在加载的场景 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Level System", meta=(AllowPrivateAccess))
	UMap* LoadingMap;

public:
	/** 正在加载的场景 */
	FORCEINLINE UMap* GetLoadingMap() const { return LoadingMap; }

private:
	/** 正在使用的场景 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Level System", meta=(AllowPrivateAccess))
	UMap* CurrentMap = nullptr;

public:
	/** 正在使用的场景 */
	FORCEINLINE UMap* GetCurrentMap() const { return CurrentMap; }

public:
	/** 场景加载的状态 */
	bool IsLoading() const;
#pragma endregion
};
