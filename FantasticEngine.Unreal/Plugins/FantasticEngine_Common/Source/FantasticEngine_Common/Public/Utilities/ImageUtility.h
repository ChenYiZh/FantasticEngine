// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialExpressionAntialiasedTextureMask.h"
#include "ImageUtility.generated.h"

/**
 * 图片处理类
 */
UCLASS()
class FANTASTICENGINE_COMMON_API UImageUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 直接从字节流创建Texture */
	UFUNCTION(Exec, Category="Fantastic Engine|Image Utility")
	static UTexture2D* CreateTexture2DFromRawData(const int32 Width, const int32 Height, const FBigBuffer& RawData,
	                                              const EPixelFormat& PixelFormat = EPixelFormat::PF_B8G8R8A8);

	/** 贴图转字节流 */
	UFUNCTION(Exec, Category="Fantastic Engine|Image Utility")
	static void ReadRawDataFromTexture2D(const UTexture2D* Texture,
	                                     int32& Width, int32& Height, FBigBuffer& RawData);

	/** 直接从像素值创建Texture */
	UFUNCTION(Exec, Category="Fantastic Engine|Image Utility")
	static UTexture2D* CreateTexture2DFromColors(const int32 Width, const int32 Height, const TArray<FColor>& Colors);

	/** 贴图转字节流 */
	UFUNCTION(Exec, Category="Fantastic Engine|Image Utility")
	static void ReadColorsFromTexture2D(const UTexture2D* Texture,
	                                    int32& Width, int32& Height, TArray<FColor>& OutColors);

	/** 文件的字节流转2D贴图 */
	UFUNCTION(Exec, Category="Fantastic Engine|Image Utility")
	static UTexture2D* LoadTexture2DFromBytes(const FBigBuffer& Buffer);

	/** 将贴图转为PNG的字节流 */
	UFUNCTION(Exec, Category="Fantastic Engine|Image Utility")
	static void CompressTexture2DToPNGBuffer(const UTexture2D* Texture, FBigBuffer& OutBuffer);

	/** 字节流转动态贴图（无源文件类型的贴图） */
	UFUNCTION(Exec, Category="Fantastic Engine|Image Utility")
	static UTexture2DDynamic* LoadTextureDynamicFromBytes(const FBigBuffer& Buffer);

	/** 将Source贴图中的SourceChannel通道复制到Target贴图中的TargetChannel通道中。贴图尺寸必须相同 */
	UFUNCTION(BlueprintCallable, Exec, Category="Fantastic Engine|Image Utility")
	static void Combine(UTexture2D* Target, ETextureColorChannel TargetChannel, UTexture2D* Source,
	                    ETextureColorChannel SourceChannel);

	/** 遍历贴图中的Alpha通道判断有没有数据 */
	UFUNCTION(BlueprintCallable, Exec, Category="Fantastic Engine|Image Utility")
	static bool HasAlphaData(UTexture2D* Texture);

	/** UTextureRenderTarget2D 转 Texture2D */
	UFUNCTION(Exec, BlueprintCallable, Category="Fantastic Engine|Image Utility",
		meta=(HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UTexture2D* ConvertTexture2DFromRenderTarget2D(UObject* WorldContextObject, UTextureRenderTarget2D* Source);

	/** UTextureRenderTarget2D 转 Texture2D */
	UFUNCTION(Exec, BlueprintCallable, Category="Fantastic Engine|Image Utility",
		meta=(HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UTextureRenderTarget2D* ConvertRenderTarget2DFromTexture2D(UObject* WorldContextObject, UTexture2D* Source,
	                                                                  int32 SizeX = 0, int32 SizeY = 0);
};
