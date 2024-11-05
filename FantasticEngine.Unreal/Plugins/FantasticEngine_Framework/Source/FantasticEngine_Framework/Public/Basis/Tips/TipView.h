// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/AnimConfig.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "TipView.generated.h"
class UTipParam;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASTICENGINE_FRAMEWORK_API UTipView : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float Seconds = 0;
	
	UPROPERTY()
	FString KeyName;
public:
	/**
	 * 注册的名称
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	FString GetKeyName();
	void SetKeyName(const FString& InKeyName);

	/**
	 * 显示面板时是否关闭其他面板
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHideOthers = true;

	/**
	 * 是否忽视被动关闭面板
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIgnoreHideOthers = false;

	/**
	 * 是否一启动就初始化
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bInitializeOnBegin = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimConfig AnimConfig;

	/**
	 * 是否初始化完成
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bInitialized;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnShow(UTipParam* Param);
	virtual void OnShow_Implementation(UTipParam* Param);

	UFUNCTION(BlueprintNativeEvent)
	void OnHide();
	virtual void OnHide_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnInitialize();
	virtual void OnInitialize_Implementation();

	/**
	* 在播放显示动画前调用
	* @param AnimIn 
	* @param AnimInSeconds 
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowBegin(UPARAM(ref) EUIAnimType& AnimIn,UPARAM(ref) float& AnimInSeconds);
	virtual void ShowBegin_Implementation(EUIAnimType& AnimIn, float& AnimInSeconds);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowEnd();
	virtual void ShowEnd_Implementation();

	/**
	 * 自定义显示动画，每帧执行，传入0-1的值，总时长需要预先设定
	 * 返回True说明动画执行完毕。可作为检测使用
	 * @param Alpha 差值，与总时间对应
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CustomAnimInLoop(float Alpha);
	/**
	* 自定义显示动画，每帧执行，传入0-1的值，总时长需要预先设定
	* 返回True说明动画执行完毕。可作为检测使用
	* @param Alpha 差值，与总时间对应
	*/
	virtual void CustomAnimInLoop_Implementation(float Alpha);

	/**
	* 在播放隐藏动画前调用
	* @param AnimOut 
	* @param AnimOutSeconds 
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HideBegin(UPARAM(ref) EUIAnimType& AnimOut,UPARAM(ref) float& AnimOutSeconds);
	virtual void HideBegin_Implementation(EUIAnimType& AnimOut, float& AnimOutSeconds);

	/**
	* 自定义隐藏动画，每帧执行，传入0-1的值，总时长需要预先设定
	* 返回True说明动画执行完毕。可作为检测使用
	* @param Alpha 差值，与总时间对应
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CustomAnimOutLoop(float Alpha);
	/**
	* 自定义隐藏动画，每帧执行，传入0-1的值，总时长需要预先设定
	* 返回True说明动画执行完毕。可作为检测使用
	* @param Alpha 差值，与总时间对应
	*/
	virtual void CustomAnimOutLoop_Implementation(float Alpha);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnQuit();
	virtual void OnQuit_Implementation();

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
