// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/InputSystem.h"

#include "GameRoot.h"
#include "Kismet/GameplayStatics.h"

bool UInputSystem::CanTap(const UObject* WorldContextObject, bool bRefresh)
{
	UInputSystem* System = Singleton<UInputSystem>(WorldContextObject);
	bool bResult = (FDateTime::Now() - System->LastTapTime).GetTotalSeconds() > 0.1f;
	if (bResult && bRefresh)
	{
		System->LastTapTime = FDateTime::Now();
	}
	return bResult;
}

void UInputSystem::SetMouseType(const UObject* WorldContextObject, EMouseType NewMouseType, const bool bChangeCache)
{
	if (UGameDefines* Defines = GetGameDefines<UGameDefines>(WorldContextObject))
	{
		Defines->SetMouseType(NewMouseType, bChangeCache);
	}
}

void UInputSystem::SetCursorVisible(const UObject* WorldContextObject, const bool bVisible)
{
	UInputSystem* System = Singleton<UInputSystem>(WorldContextObject);
	System->bCursorVisible = bVisible;
	APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(WorldContextObject, 0);
	if (Controller != nullptr)
	{
		UGameViewportClient* GameViewportClient = Controller->GetWorld()->GetGameViewport();
		GameViewportClient->SetMouseLockMode(bVisible ? EMouseLockMode::DoNotLock : EMouseLockMode::LockAlways);
		GameViewportClient->SetIgnoreInput(false);
		GameViewportClient->SetMouseCaptureMode(bVisible
			                                        ? EMouseCaptureMode::NoCapture
			                                        : EMouseCaptureMode::CapturePermanently);
		Controller->SetShowMouseCursor(bVisible);
	}
}

bool UInputSystem::IsCursorVisible(const UObject* WorldContextObject)
{
	const UInputSystem* System = Singleton<UInputSystem>(WorldContextObject);
	if (!IsValid(System))
	{
		return false;
	}
	return System->bCursorVisible;
}
