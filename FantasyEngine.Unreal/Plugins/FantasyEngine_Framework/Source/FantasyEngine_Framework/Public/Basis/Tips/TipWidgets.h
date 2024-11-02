// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TipLoadingBasic.h"
#include "TipMessageBoxBasic.h"
#include "TipToastBasic.h"
#include "TipView.h"
#include "Blueprint/UserWidget.h"
#include "TipWidgets.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UTipParam : public UWorldObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FTipAnimAction
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bShowing;
	UPROPERTY()
	EUIAnimType AnimType;
	UPROPERTY()
	float Seconds;
	UPROPERTY()
	float TotalSeconds;
	UPROPERTY()
	UTipView* TipView;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UTipWidgets : public UUserWidget
{
	GENERATED_BODY()

private:
	inline static UTipWidgets* Root;

private:
	UPROPERTY()
	float Seconds = 0;

public:
	UFUNCTION(BlueprintPure, Category="Tips")
	static UTipWidgets* GetRoot();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FString, UTipView*> Views;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FString, FTipAnimAction> TipActions;

public:
	virtual void BeginInitialize();

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnInitialize();
	virtual void OnInitialize_Implementation();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void Registing();
	virtual void Registing_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void ShowImpl(const FString& TipKeyName, bool bHideOthers = true, UTipParam* Param = nullptr);
	virtual void ShowImpl_Implementation(const FString& TipKeyName, bool bHideOthers = true,
	                                     UTipParam* Param = nullptr);

	UFUNCTION(BlueprintNativeEvent)
	void OnShowEnd(UTipView* TipView);
	virtual void OnShowEnd_Implementation(UTipView* TipView);

	UFUNCTION(BlueprintNativeEvent)
	void HideImpl(const FString& TipKeyName);
	virtual void HideImpl_Implementation(const FString& TipKeyName);

	UFUNCTION(BlueprintNativeEvent)
	void OnHideEnd(UTipView* TipView);
	virtual void OnHideEnd_Implementation(UTipView* TipView);

protected:
	void PlayTipAnim(UTipView* View, EUIAnimType AnimType, bool bOpening, float Alpha);
	UFUNCTION(BlueprintPure)
	bool IsViewVisible(UTipView* View);

public:
	UFUNCTION(BlueprintCallable, Category="Tip Widgets", meta=(WorldContext="WorldContextObject"))
	static void Show(const UObject* WorldContextObject, const FString& TipKeyName, UTipParam* Param = nullptr,
	                 const bool& bHideOthers = true);

	UFUNCTION(BlueprintCallable, Category="Tip Widgets", meta=(WorldContext="WorldContextObject"))
	static void Hide(const UObject* WorldContextObject, const FString& TipKeyName);

	UFUNCTION(BlueprintCallable, Category="Tip Widgets", meta=(WorldContext="WorldContextObject"))
	static void HideAllTips(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnQuit();
	virtual void OnQuit_Implementation();

protected:
	UFUNCTION(BlueprintCallable, Category="Tip Widgets")
	void Regist(const FString& TipKeyName, UTipView* TipVIew);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEverySecond();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
