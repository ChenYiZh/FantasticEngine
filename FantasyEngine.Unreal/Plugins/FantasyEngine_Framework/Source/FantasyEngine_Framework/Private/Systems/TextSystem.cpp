// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/TextSystem.h"

#include "Utilities/TextUtility.h"

void UTextSystem::OnInitialize_Implementation()
{
	UTextUtility::SetTextLanguage(UTextUtility::GetTextLanguage());
}
