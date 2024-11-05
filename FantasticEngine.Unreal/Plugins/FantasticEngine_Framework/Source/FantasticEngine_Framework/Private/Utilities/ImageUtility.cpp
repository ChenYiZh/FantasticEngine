// Fill out your copyright notice in the Description page of Project Settings.
#include "..\..\Public\Utilities\ImageUtility.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2DDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Log/FEConsole.h"
#include "Utilities/ObjectFactory.h"

UTexture2D* UImageUtility::LoadTexture2DFromBytes(const TArray<uint8>& Buffer)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
	TSharedPtr<IImageWrapper> ImageWrappers[3] =
	{
		ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
	};
	for (const TSharedPtr<IImageWrapper>& ImageWrapper : ImageWrappers)
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Buffer.GetData(), Buffer.Num()))
		{
			TArray64<uint8> RawData;
			ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawData);
			int32 Width = ImageWrapper->GetWidth();
			int32 Height = ImageWrapper->GetHeight();

			UTexture2D* Result = UTexture2D::CreateTransient(Width, Height, EPixelFormat::PF_R8G8B8A8);

			FTexture2DMipMap& MipMap = Result->GetPlatformData()->Mips[0];
			void* Data = MipMap.BulkData.Lock(LOCK_READ_WRITE);

			FMemory::Memcpy(Data, RawData.GetData(), RawData.Num());

			MipMap.BulkData.Unlock();
			Result->UpdateResource();

			return Result;
		}
	}
	return nullptr;
}

UTexture2DDynamic* UImageUtility::LoadTextureDynamicFromBytes(const TArray<uint8>& Buffer)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(
		FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrappers[3] =
	{
		ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
	};

	for (const TSharedPtr<IImageWrapper>& ImageWrapper : ImageWrappers)
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Buffer.GetData(), Buffer.Num()))
		{
			TArray64<uint8> RawData;
			const ERGBFormat InFormat = ERGBFormat::BGRA;
			if (ImageWrapper->GetRaw(InFormat, 8, RawData))
			{
				if (UTexture2DDynamic* Texture = UTexture2DDynamic::Create(
					ImageWrapper->GetWidth(), ImageWrapper->GetHeight()))
				{
					Texture->SRGB = true;
					Texture->UpdateResource();

					FTexture2DDynamicResource* TextureResource = static_cast<FTexture2DDynamicResource*>(Texture->
						GetResource());
					if (TextureResource)
					{
						ENQUEUE_RENDER_COMMAND(FWriteRawDataToTexture)(
							[TextureResource, RawData = MoveTemp(RawData)](FRHICommandListImmediate& RHICmdList)
							{
								//TextureResource->WriteRawToTexture_RenderThread(RawData);
							});
					}
					return Texture;
				}
			}
		}
	}
	return nullptr;
}

UTexture2D* UImageUtility::ConvertTexture2DFromRenderTarget2D(UObject* WorldContextObject,
                                                              UTextureRenderTarget2D* Source)
{
	if (Source)
	{
		TArray<FColor> colors;
		UKismetRenderingLibrary::ReadRenderTarget(WorldContextObject, Source, colors);
		TArray<uint8> buffer;
		buffer.SetNumUninitialized(colors.Num() * sizeof(FColor));
		FMemory::Memcpy(buffer.GetData(), colors.GetData(), buffer.Num());
		return LoadTexture2DFromBytes(buffer);
	}
	return nullptr;
}

UTextureRenderTarget2D* UImageUtility::ConvertRenderTarget2DFromTexture2D(
	UObject* WorldContextObject, UTexture2D* Source, int32 SizeX, int32 SizeY)
{
	if (Source)
	{
		UMaterial* materialBase = LoadObject<UMaterial>(WorldContextObject,
		                                                TEXT(
			                                                "/Script/Engine.Material'/Framework/Materials/Functions/M_TextureCopy.M_TextureCopy'"));
		if (!materialBase)
		{
			UFEConsole::WriteError(TEXT(
				"Lost Material: /Script/Engine.Material'/Framework/Materials/Functions/M_TextureCopy.M_TextureCopy'"));
			return nullptr;
		}
		if (SizeX == 0)
		{
			SizeX = Source->GetSizeX();
		}
		if (SizeY == 0)
		{
			SizeY = Source->GetSizeY();
		}
		UTextureRenderTarget2D* target = UKismetRenderingLibrary::CreateRenderTarget2D(
			WorldContextObject,
			SizeX, SizeY,
			RTF_RGBA16f, FLinearColor::Transparent);
		UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(
			materialBase, WorldContextObject);
		material->SetTextureParameterValue(TEXT("Source"), Source);
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(WorldContextObject, target, material);
		UObjectFactory::Destroy(material);
		return target;
	}
	return nullptr;
}
