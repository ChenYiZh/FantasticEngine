// Fill out your copyright notice in the Description page of Project Settings.


#include "Basis/SystemBasis.h"

bool USystemBasis::EnableTickBeforeInitialized_Implementation()
{
	return false;
}

bool USystemBasis::IsReady_Implementation()
{
	return true;
}

void USystemBasis::OnInitialize_Implementation()
{
}

void USystemBasis::OnReady_Implementation()
{
}

void USystemBasis::OnLogin_Implementation()
{
}

void USystemBasis::OnLogout_Implementation()
{
}

void USystemBasis::OnReset_Implementation()
{
}

void USystemBasis::OnTick_Implementation(float DeltaTime)
{
}

void USystemBasis::OnEverySecond_Implementation()
{
}

void USystemBasis::OnApplicationFocus_Implementation(bool Focus)
{
}

void USystemBasis::OnApplicationPause_Implementation(bool Pause)
{
}

void USystemBasis::OnQuit_Implementation()
{
}
