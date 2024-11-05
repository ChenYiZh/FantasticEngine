// Fill out your copyright notice in the Description page of Project Settings.


#include "Defines/InputDefines.h"

#include "EnhancedInputSubsystems.h"
#include "Log/FEConsole.h"

#pragma region 初始化操作
void UInputDefines::Initialize()
{
	UObject::PostInitProperties();
	UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetWorld()->GetFirstLocalPlayerFromController());
	for (int32 i = 0; i < InputMappingContexts.Num(); i++)
	{
		UInputMappingContext* Context = InputMappingContexts[i];
		System->AddMappingContext(Context, i);
	}
}
#pragma endregion
