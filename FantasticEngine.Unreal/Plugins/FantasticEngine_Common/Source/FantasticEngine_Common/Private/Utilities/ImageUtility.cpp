// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/ImageUtility.h"

#include "BigBuffer.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "UnrealUSDWrapper.h"
#include "Common/SizeUtility.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Log/FEConsole.h"


const int32 USizeUtility::BoolSize = sizeof(bool);
//const int32 USizeUtility::CharSize = sizeof(TCHAR);

const int32 USizeUtility::FloatSize = sizeof(float);
const int32 USizeUtility::DoubleSize = sizeof(double);

const int32 USizeUtility::SByteSize = sizeof(int8);
const int32 USizeUtility::ShortSize = sizeof(int16);
const int32 USizeUtility::IntSize = sizeof(int32);
const int32 USizeUtility::LongSize = sizeof(int64);

const int32 USizeUtility::ByteSize = sizeof(uint8);
const int32 USizeUtility::UShortSize = sizeof(uint16);
const int32 USizeUtility::UIntSize = sizeof(uint32);
const int32 USizeUtility::ULongSize = sizeof(uint64);

const int32 USizeUtility::ANSICHARSize = sizeof(ANSICHAR);
//const int32 USizeUtility::CHARSize = sizeof(CHAR);
const int32 USizeUtility::TCHARSize = sizeof(TCHAR);


const int32 USizeUtility::COLORSIZE = sizeof(FColor);

UTexture2D* UImageUtility::CreateTexture2DFromRawData(const int32 Width, const int32 Height,
                                                      const FBigBuffer& RawData, const EPixelFormat& PixelFormat)
{
	UTexture2D* Result = UTexture2D::CreateTransient(Width, Height, PixelFormat);

	FTexture2DMipMap& MipMap = Result->GetPlatformData()->Mips[0];
	void* Data = MipMap.BulkData.Lock(LOCK_READ_WRITE);

	FMemory::Memcpy(Data, RawData.Buffer->GetData(), RawData.Buffer->Num());

	MipMap.BulkData.Unlock();
	Result->UpdateResource();

	return Result;
}

void UImageUtility::ReadRawDataFromTexture2D(const UTexture2D* Texture, int32& Width, int32& Height,
                                             FBigBuffer& RawData)
{
	Width = Texture->GetSizeX();
	Height = Texture->GetSizeY();
	const FTexture2DMipMap& MipMap = Texture->GetPlatformData()->Mips[0];
	const void* Data = MipMap.BulkData.LockReadOnly();
	int64 Size = Width * Height * USizeUtility::COLORSIZE;
	RawData.Buffer->AddUninitialized(Size);
	FMemory::Memcpy(RawData.Buffer->GetData(), Data, Size);

	MipMap.BulkData.Unlock();
}

UTexture2D* UImageUtility::CreateTexture2DFromColors(const int32 Width, const int32 Height,
                                                     const TArray<FColor>& Colors)
{
	UTexture2D* Result = UTexture2D::CreateTransient(Width, Height, EPixelFormat::PF_R8G8B8A8);

	FTexture2DMipMap& MipMap = Result->GetPlatformData()->Mips[0];
	void* Data = MipMap.BulkData.Lock(LOCK_READ_WRITE);

	FMemory::Memcpy(Data, Colors.GetData(), Colors.Num() * USizeUtility::COLORSIZE);

	MipMap.BulkData.Unlock();
	Result->UpdateResource();

	return Result;
}

void UImageUtility::ReadColorsFromTexture2D(const UTexture2D* Texture, int32& Width, int32& Height,
                                            TArray<FColor>& OutColors)
{
	Width = Texture->GetSizeX();
	Height = Texture->GetSizeY();
	OutColors.SetNumUninitialized(Width * Height);
	const FTexture2DMipMap& MipMap = Texture->GetPlatformData()->Mips[0];
	const void* Data = MipMap.BulkData.LockReadOnly();
	FMemory::Memcpy(OutColors.GetData(), Data, Width * Height * USizeUtility::COLORSIZE);
	MipMap.BulkData.Unlock();
}

UTexture2D* UImageUtility::LoadTexture2DFromBytes(const FBigBuffer& Buffer)
{
	FImage Image;
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(Buffer.Buffer->GetData(), Buffer.Buffer->Num());
	// UFEConsole::Write(FString::Printf(TEXT("Format: %d"), ImageFormat));
	// if (FImageUtils::DecompressImage(Buffer.Buffer->GetData(), Buffer.Buffer->Num(), Image))
	// {
	// 	int32 Width = Image.GetWidth();
	// 	int32 Height = Image.GetHeight();
	//
	// 	return CreateTexture2DFromRawData(Width, Height, Image.RawData);
	// }
	//IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");

	TSharedPtr<IImageWrapper> ImageWrapper;
	switch (ImageFormat)
	{
	case EImageFormat::PNG:
	case EImageFormat::JPEG:
	case EImageFormat::BMP:
		ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
		break;
	default: ImageWrapper = nullptr;
	}
	// TSharedPtr<IImageWrapper> ImageWrappers[3] =
	// {
	// 	ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
	// 	ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
	// 	ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
	// };
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Buffer.Buffer->GetData(), Buffer.Buffer->Num()))
	{
		FBigBuffer RawData;
		ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData.Buffer.Get());
		int32 Width = ImageWrapper->GetWidth();
		int32 Height = ImageWrapper->GetHeight();

		return CreateTexture2DFromRawData(Width, Height, RawData);
	}
	return nullptr;
}

void UImageUtility::CompressTexture2DToPNGBuffer(const UTexture2D* Texture, FBigBuffer& OutBuffer)
{
	TArray<FColor> Colors;
	int32 Width, Height;
	ReadColorsFromTexture2D(Texture, Width, Height, Colors);
	//FImageUtils::PNGCompressImageArray(Width, Height, Colors, *OutBuffer.Buffer);
	FImageView Image(Colors.GetData(), Width, Height, ERawImageFormat::Type::BGRA8);
	//FImageCore::TransposeImageRGBABGRA(Image);
	FImageUtils::CompressImage(*OutBuffer.Buffer,TEXT("png"), Image);
}

UTexture2DDynamic* UImageUtility::LoadTextureDynamicFromBytes(const FBigBuffer& Buffer)
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
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Buffer.Buffer->GetData(), Buffer.Buffer->Num()))
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

void UImageUtility::Combine(UTexture2D* Target, ETextureColorChannel TargetChannel, UTexture2D* Source,
                            ETextureColorChannel SourceChannel)
{
	FTexture2DMipMap& TargetMipMap = Target->GetPlatformData()->Mips[0];
	FColor* TargetData = static_cast<FColor*>(TargetMipMap.BulkData.Lock(LOCK_READ_WRITE));
	int32 Length = TargetMipMap.BulkData.GetElementCount() / USizeUtility::COLORSIZE;
	// TArray<FColor> TargetColors;
	// FMemory::Memcpy(TargetColors.GetData(), TargetData, Length);
	FTexture2DMipMap& SourceMipMap = Source->GetPlatformData()->Mips[0];
	const FColor* SourceData = static_cast<const FColor*>(SourceMipMap.BulkData.LockReadOnly());
	// TArray<FColor> SourceColors;
	// FMemory::Memcpy(SourceColors.GetData(), SourceData, Length);
	for (int32 Index = 0; Index < Length; Index++)
	{
		uint8 SrcColor = 0;
		// ReSharper disable once CppIncompleteSwitchStatement
		// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
		switch (SourceChannel)
		{
		case ETextureColorChannel::TCC_Red: SrcColor = SourceData[Index].R;
			break;
		case ETextureColorChannel::TCC_Green: SrcColor = SourceData[Index].G;
			break;
		case ETextureColorChannel::TCC_Blue: SrcColor = SourceData[Index].B;
			break;
		case ETextureColorChannel::TCC_Alpha: SrcColor = SourceData[Index].A;
			break;
		}
		// ReSharper disable once CppIncompleteSwitchStatement
		// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
		switch (TargetChannel)
		{
		case ETextureColorChannel::TCC_Red: TargetData[Index].R = SrcColor;
			break;
		case ETextureColorChannel::TCC_Green: TargetData[Index].G = SrcColor;
			break;
		case ETextureColorChannel::TCC_Blue: TargetData[Index].B = SrcColor;
			break;
		case ETextureColorChannel::TCC_Alpha: TargetData[Index].A = SrcColor;
			break;
		}
	}
	//FMemory::Memcpy(TargetData, TargetColors.GetData(), Length * USizeUtility::COLORSIZE);
	SourceMipMap.BulkData.Unlock();
	TargetMipMap.BulkData.Unlock();
	Target->UpdateResource();
}

bool UImageUtility::HasAlphaData(UTexture2D* Texture)
{
	if (!IsValid(Texture)) { return false; }
	FTexture2DMipMap& MipMap = Texture->GetPlatformData()->Mips[0];
	int32 Length = MipMap.BulkData.GetElementCount() / USizeUtility::COLORSIZE;
	const FColor* Color = static_cast<const FColor*>(MipMap.BulkData.LockReadOnly());
	bool bHasAlpha = false;
	for (int32 Index = 0; Index < Length; Index++)
	{
		if (Color[Index].A < MAX_uint8)
		{
			bHasAlpha = true;
			break;
		}
	}
	MipMap.BulkData.Unlock();
	return bHasAlpha;
}

UTexture2D* UImageUtility::ConvertTexture2DFromRenderTarget2D(UObject* WorldContextObject,
                                                              UTextureRenderTarget2D* Source)
{
	if (Source)
	{
		TArray<FColor> colors;
		UKismetRenderingLibrary::ReadRenderTarget(WorldContextObject, Source, colors);
		FBigBuffer buffer;
		int64 Size = colors.Num() * USizeUtility::COLORSIZE;
		buffer.Buffer->SetNumUninitialized(Size);
		FMemory::Memcpy(buffer.Buffer->GetData(), colors.GetData(), Size);
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
		if (material->IsRooted())
		{
			material->RemoveFromRoot();
		}
		material->ClearGarbage();
		material->MarkAsGarbage();
		return target;
	}
	return nullptr;
}
