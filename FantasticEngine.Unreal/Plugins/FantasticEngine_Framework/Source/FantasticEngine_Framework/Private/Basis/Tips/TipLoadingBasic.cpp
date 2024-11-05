// Fill out your copyright notice in the Description page of Project Settings.


#include "Basis/Tips/TipLoadingBasic.h"

#include "Basis/Tips/TipWidgets.h"
#include "Defines/EventIdsBasic.h"
#include "Defines/TipKeysBasic.h"

void UTipLoadingBasic::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
	UEventSystem::RegistEvent(this, UEventIdsBasic::SHOW_LOADING(), this, "ShowListener");
	UEventSystem::RegistEvent(this, UEventIdsBasic::LOADING_PROGRESS(), this, "ProgressListener");
	UEventSystem::RegistEvent(this, UEventIdsBasic::HIDE_LOADING(), this, "HideListener");
}

void UTipLoadingBasic::OnQuit_Implementation()
{
	UEventSystem::UnregistEvent(this, UEventIdsBasic::SHOW_LOADING(), this, "ShowListener");
	UEventSystem::UnregistEvent(this, UEventIdsBasic::LOADING_PROGRESS(), this, "ProgressListener");
	UEventSystem::UnregistEvent(this, UEventIdsBasic::HIDE_LOADING(), this, "HideListener");
	Super::OnQuit_Implementation();
}

void UTipLoadingBasic::ShowListener()
{
	UTipWidgets::Show(this, UTipKeysBasic::LOADING());
}

void UTipLoadingBasic::ProgressListener(UEventParam* Param)
{
	ULoadingParam* LoadingParam = Cast<ULoadingParam>(Param);
	if (LoadingParam != nullptr)
	{
		OnProgress(LoadingParam->Progress, LoadingParam->Message);
	}
}

void UTipLoadingBasic::HideListener()
{
	UTipWidgets::Hide(this, UTipKeysBasic::LOADING());
}

void UTipLoadingBasic::OnProgress_Implementation(float Progress, const FString& Message)
{
}
