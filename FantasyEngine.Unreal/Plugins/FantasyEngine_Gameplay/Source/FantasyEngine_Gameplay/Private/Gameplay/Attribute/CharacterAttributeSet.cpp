// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Attribute/CharacterAttributeSet.h"

#include "Log/FEConsole.h"

bool UCharacterAttributeSet::IsEmptyHealth_Implementation()
{
	UFEConsole::WriteWarnWithCategory(TEXT("Attributes"),TEXT("You should override the function: IsEmptyHealth"));
	return true;
}
