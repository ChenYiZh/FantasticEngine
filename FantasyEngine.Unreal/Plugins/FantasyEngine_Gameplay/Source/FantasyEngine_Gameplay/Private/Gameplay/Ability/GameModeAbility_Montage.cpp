// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Ability/GameModeAbility_Montage.h"

void UGameModeAbility_Montage::ExecuteMontageBegin(UAnimMontage* MontagePlaying)
{
	OnMontageBegin(MontagePlaying);
}

void UGameModeAbility_Montage::ExecuteMontageEnd(UAnimMontage* MontagePlaying)
{
	OnMontageEnd(MontagePlaying);
}

void UGameModeAbility_Montage::OnMontageBegin_Implementation(UAnimMontage* MontagePlaying)
{
}

void UGameModeAbility_Montage::OnMontageEnd_Implementation(UAnimMontage* MontagePlaying)
{
}

void UGameModeAbility_Montage::ExecuteNofityBegin(UAnimMontage* MontagePlaying, FName Name,
                                                  const FTransparentParameters& Parameters)
{
	OnNofityBegin(MontagePlaying, Name, Parameters);
}

void UGameModeAbility_Montage::ExecuteNofityEnd(UAnimMontage* MontagePlaying, FName Name,
                                                const FTransparentParameters& Parameters)
{
	OnNofityEnd(MontagePlaying, Name, Parameters);
}

void UGameModeAbility_Montage::OnNofityBegin_Implementation(UAnimMontage* MontagePlaying, FName Name,
                                                            const FTransparentParameters& Parameters)
{
}

void UGameModeAbility_Montage::OnNofityEnd_Implementation(UAnimMontage* MontagePlaying, FName Name,
                                                          const FTransparentParameters& Parameters)
{
}
