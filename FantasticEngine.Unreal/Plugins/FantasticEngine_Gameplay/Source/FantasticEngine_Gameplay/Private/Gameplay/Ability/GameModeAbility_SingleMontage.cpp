// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Ability/GameModeAbility_SingleMontage.h"

void UGameModeAbility_SingleMontage::OnActive()
{
	if (bAutoPlayOnActive)
	{
		UGameplayTask_PlayMontage::CreateTaskPlaySingleMontageProxy(this, NAME_None, MontagePlayRate, StartSection,
		                                                            bStopWhenAbilityEnds, StartTimeSeconds);
	}
	Super::OnActive();
}

void UGameModeAbility_SingleMontage::OnMontageEnd_Implementation(UAnimMontage* MontagePlaying)
{
	Super::OnMontageEnd_Implementation(MontagePlaying);
	if (bAutoFinishOnMontageCompleted)
	{
		Finish();
	}
}

UAnimMontage* UGameModeAbility_SingleMontage::GetMontageToPlay() const
{
	return MontageToPlay;
}
