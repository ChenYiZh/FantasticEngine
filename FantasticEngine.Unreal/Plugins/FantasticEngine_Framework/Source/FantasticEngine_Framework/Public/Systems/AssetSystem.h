// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "Engine/StreamableManager.h"
#include "AssetSystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FAssetClassCallback, UClass*, Class);

DECLARE_DYNAMIC_DELEGATE_OneParam(FAssetObjectCallback, UObject*, Object);

USTRUCT(BlueprintType)
struct FAssetHandle
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsLoading;
	UPROPERTY()
	bool bIsClass;
	UPROPERTY()
	FString FullPath;
	UPROPERTY()
	FSoftClassPath ClassPath;
	UPROPERTY()
	FAssetClassCallback ClassCallback;
	UPROPERTY()
	FSoftObjectPath ObjectPath;
	UPROPERTY()
	FAssetObjectCallback ObjectCallback;
	TSharedPtr<FStreamableHandle> StreamableHandle;
};

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASTICENGINE_FRAMEWORK_API UAssetSystem : public USystemBasis
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable)
	// UUserWidget* LoadWidget(const FString& ClassName);
	UFUNCTION(BlueprintCallable)
	void LoadWidgetAsync(const FString& ClassPath, FAssetClassCallback Callback);

private:
	UPROPERTY()
	FAssetHandle AssetHandle;
	TSharedPtr<TQueue<TSharedPtr<FAssetHandle>>> TaskQueue
		= MakeShared<TQueue<TSharedPtr<FAssetHandle>>>();

public:
	virtual void OnInitialize_Implementation() override;
	virtual void OnTick_Implementation(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	void LoadUClassAsyncImpl(FString FullPath, FAssetClassCallback Callback);

private:
	void BeginLoadUClassAsync(const FAssetHandle& AssetClassHandle);

private:
	UFUNCTION()
	void OnUClassLoaded();

protected:
	UFUNCTION(BlueprintCallable)
	void LoadUObjectAsyncImpl(FString FullPath, FAssetObjectCallback Callback);

private:
	void BeginLoadUObjectAsync(const FAssetHandle& AssetClassHandle);

private:
	UFUNCTION()
	void OnUObjectLoaded();

public:
	UFUNCTION(BlueprintCallable)
	void Release(bool IncludeTaskQueue = false);

public:
	/**
	 * 异步加载
	 * @param FullPath 
	 * @param Callback 
	 */
	UFUNCTION(BlueprintCallable, Category="Asset System", meta=(WorldContext="WorldContextObject"))
	static void LoadUClassAsync(const UObject* WorldContextObject, const FString FullPath,
	                            FAssetClassCallback Callback);
	/**
	 * 同步加载,尽量少用
	 * @param FullPath 
	 */
	UFUNCTION(BlueprintCallable, Category="Asset System")
	static UClass* LoadUClass(FString FullPath);
	UFUNCTION(BlueprintCallable, Category="Asset System", meta=(WorldContext="WorldContextObject"))
	static void LoadUObjectAsync(const UObject* WorldContextObject, const FString FullPath,
	                             FAssetObjectCallback Callback);
};
