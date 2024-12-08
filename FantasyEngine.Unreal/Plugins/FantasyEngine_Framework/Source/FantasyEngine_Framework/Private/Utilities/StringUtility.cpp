// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/StringUtility.h"

#include "GameRoot.h"

void UStringUtility::CheckCommands(FString Text, TArray<FString>& Commands)
{
	Text.ParseIntoArray(Commands,TEXT(";"), false);
}

void UStringUtility::CheckMontageNotify(FName Name, FName& Notify, TArray<FString>& Commands)
{
	CheckCommands(Name.ToString(), Commands);
	if (Commands.Num() > 0)
	{
		Notify = FName(Commands[0]);
		Commands.RemoveAt(0);
	}
	else
	{
		Notify = NAME_None;
	}
}