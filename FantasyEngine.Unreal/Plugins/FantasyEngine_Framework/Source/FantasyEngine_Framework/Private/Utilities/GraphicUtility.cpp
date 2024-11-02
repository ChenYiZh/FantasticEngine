#include "Utilities/GraphicUtility.h"

#include "GameRoot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Defines/Settings.h"
#include "Kismet/GameplayStatics.h"
#include "Log/FEConsole.h"

FString UGraphicUtility::GetSectionName()
{
	return TEXT("Graphics");
}

EWindowMode::Type UGraphicUtility::GetWindowMode()
{
	int32 WindowMode = USettings::GetInt(GetSectionName(),TEXT("WindowMode"), GSystemResolution.WindowMode);
	return EWindowMode::ConvertIntToWindowMode(WindowMode);
}

EWindowMode::Type UGraphicUtility::SetWindowMode(EWindowMode::Type NewWindowMode)
{
	USettings::SetInt(GetSectionName(),TEXT("WindowMode"), NewWindowMode);
	GSystemResolution.WindowMode = NewWindowMode;
	return NewWindowMode;
}

FVector2D UGraphicUtility::GetViewportSize(const UObject* WorldContextObject)
{
	if (!IsValid(UGameRoot::GetRootWorld(WorldContextObject)))
	{
		return FVector2D::Zero();
	}
	const FIntPoint XY = UGameRoot::GetRootWorld(WorldContextObject)->GetGameViewport()->Viewport->GetSizeXY();
	return FVector2D(XY.X, XY.Y);
}

float UGraphicUtility::GetViewportScale(const UObject* WorldContextObject)
{
	return UWidgetLayoutLibrary::GetViewportScale(UGameRoot::GetRootWorld(WorldContextObject)->GetGameViewport());
}

FVector2D UGraphicUtility::GetScreenSize(const UObject* WorldContextObject)
{
	if (IsEngineExitRequested())
	{
		return FVector2D::ZeroVector;
	}
	const FVector2D InScreenSize = GetViewportSize(WorldContextObject);
	const float ScreenScale = GetViewportScale(WorldContextObject);
	const FVector2D ScreenSize = InScreenSize / ScreenScale;
	return ScreenSize;
}

bool UGraphicUtility::IsHairScreen(const UObject* WorldContextObject)
{
	const UGameDefines* Defines = UGameRoot::GetGameDefines(WorldContextObject);
	if (Defines->UseHairScreenAdapter())
	{
		const FVector2D ScreenSize = GetScreenSize(WorldContextObject);
		return ScreenSize.X / ScreenSize.Y > Defines->MaxHairScreenResolution.X / Defines->MaxHairScreenResolution.Y;
	}
	return false;
}

bool UGraphicUtility::IsWideScreen(const UObject* WorldContextObject)
{
	const UGameDefines* Defines = UGameRoot::GetGameDefines(WorldContextObject);
	if (Defines->UseWideScreenAdapter())
	{
		const FVector2D ScreenSize = GetScreenSize(WorldContextObject);
		return ScreenSize.X / ScreenSize.Y > Defines->MaxWideScreenResolution.X / Defines->MaxWideScreenResolution.Y;
	}
	return false;
}

void UGraphicUtility::PlayUIAnim(UWidget* Widget, EUIAnimType AnimType, bool bInvert, float Alpha)
{
	switch (AnimType)
	{
	case EUIAnimType::Opacity:
		{
			Widget->SetRenderOpacity(bInvert ? 1.f - Alpha : Alpha);
		}
		break;
	case EUIAnimType::Scale:
		{
			Widget->SetRenderScale(FVector2D::One() * (bInvert ? 1 - Alpha : Alpha));
		}
		break;
	case EUIAnimType::Round:
		{
			UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget);
			if (Slot != nullptr)
			{
				float Min = FMath::Lerp(-1.f, 0.f, bInvert ? 1 - Alpha : Alpha);
				float Max = FMath::Lerp(2.f, 1.f, bInvert ? 1 - Alpha : Alpha);
				Slot->SetAnchors(FAnchors(Min, Min, Max, Max));
			}
		}
		break;
	}
}

FVector2D UGraphicUtility::FillScreenWithOriginRate(const UObject* WorldContextObject, const FVector2D InSize)
{
	const FVector2D ScreenSize = GetScreenSize(WorldContextObject);
	FVector2D ResultSize = InSize;
	if (ScreenSize.X / ScreenSize.Y > InSize.X / InSize.Y)
	{
		const float Scale = ScreenSize.X / InSize.X;
		ResultSize = ResultSize * Scale;
	}
	else
	{
		const float Scale = ScreenSize.Y / InSize.Y;
		ResultSize = ResultSize * Scale;
	}
	return ResultSize + FVector2D::One() * 2; //边缘补充两个像素
}

bool UGraphicUtility::IsVisible(ESlateVisibility Visibility)
{
	return !(Visibility == ESlateVisibility::Collapsed || Visibility == ESlateVisibility::Hidden);
}

bool UGraphicUtility::InScreen(const UObject* WorldContextObject, FVector WorldLocation, bool bOnlyProjection)
{
	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		FVector2D ScreenPosition;
		if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition))
		{
			if (bOnlyProjection)
			{
				return true;
			}
			const FVector2D ScreenSize = GetScreenSize(WorldContextObject);
			return ScreenPosition.X >= 0 && ScreenPosition.X <= ScreenSize.X && ScreenPosition.Y >= 0
				&& ScreenPosition.Y <= ScreenSize.Y;
		}
		return false;
	}
	return false;
}
