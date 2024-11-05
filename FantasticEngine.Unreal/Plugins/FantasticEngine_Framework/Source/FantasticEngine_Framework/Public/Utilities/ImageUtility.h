// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImageUtility.generated.h"

/**
 * 图片处理类
 */
UCLASS()
class FANTASTICENGINE_FRAMEWORK_API UImageUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 字节流转2D贴图 
	 */
	UFUNCTION(Exec, Category="Image Utility")
	static UTexture2D* LoadTexture2DFromBytes(const TArray<uint8>& Buffer);

	/**
	 * 字节流转动态贴图（无源文件类型的贴图） 
	 */
	UFUNCTION(Exec, Category="Image Utility")
	static UTexture2DDynamic* LoadTextureDynamicFromBytes(const TArray<uint8>& Buffer);

	/**
	 * UTextureRenderTarget2D 转 Texture2D
	 */
	UFUNCTION(Exec, BlueprintCallable, Category="Image Utility",
		meta=(HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UTexture2D* ConvertTexture2DFromRenderTarget2D(UObject* WorldContextObject, UTextureRenderTarget2D* Source);

	/**
	 * UTextureRenderTarget2D 转 Texture2D
	 */
	UFUNCTION(Exec, BlueprintCallable, Category="Image Utility",
		meta=(HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UTextureRenderTarget2D* ConvertRenderTarget2DFromTexture2D(UObject* WorldContextObject, UTexture2D* Source,
	                                                                  int32 SizeX = 0, int32 SizeY = 0);
};
