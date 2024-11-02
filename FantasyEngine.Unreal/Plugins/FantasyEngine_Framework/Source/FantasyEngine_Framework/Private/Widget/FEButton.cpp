// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FEButton.h"

#include "Systems/InputSystem.h"

void UFEButton::OnTappedBroadcast() const
{
	if (UInputSystem::CanTap(this))
	{
		OnTapped.Broadcast();
	}
}

void UFEButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	if (bClearClickEventOnInitialize)
	{
		OnClicked.Clear();
	}
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, FName("OnTappedBroadcast"));
	OnClicked.Add(Delegate);
}

void UFEButton::BeginDestroy()
{
	Super::BeginDestroy();
	OnClicked.Remove(this, FName("OnTappedBroadcast"));
}
