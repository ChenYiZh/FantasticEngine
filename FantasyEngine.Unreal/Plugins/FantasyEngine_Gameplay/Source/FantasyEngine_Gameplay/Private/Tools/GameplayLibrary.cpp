// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/GameplayLibrary.h"

#include "Gameplay/GameEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UGameplayCharacterComponent* UGameplayLibrary::GetLocalCharacter(const UObject* WorldContextObject,
                                                                 TSubclassOf<UGameplayCharacterComponent> Class,
                                                                 const int32 ControllerId)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(
		WorldContextObject, ControllerId);

	if (IsValid(Controller))
	{
		return Controller->GetPawn()->GetComponentByClass<UGameplayCharacterComponent>();
	}
	return nullptr;
}

UGameplayPlayerControllerComponent* UGameplayLibrary::GetLocalController(
	const UObject* WorldContextObject,
	TSubclassOf<UGameplayPlayerControllerComponent> Class,
	const int32 ControllerId)
{
	APlayerController* Controller;
	// if (ControllerId == 0)
	// {
	// 	Controller = UWorldUtility::FindGameWorld()->GetFirstPlayerController();
	// }
	// else
	// {
	Controller = UGameplayStatics::GetPlayerControllerFromID(WorldContextObject, ControllerId);
	//}
	if (IsValid(Controller))
	{
		return Controller->GetComponentByClass<UGameplayPlayerControllerComponent>();
	}
	return nullptr;
}

FAttackContext UGameplayLibrary::MakeContext(AActor* Source, UGameEffect* GameEffect, const int32 TargetIndex,
                                             UGameModeAbility* SourceAbility, FTransparentParameters Parameters)
{
	FAttackContext Context;
	Context.Source = Source;
	if (IsValid(GameEffect))
	{
		Context.Source = GameEffect->Source;
	}
	Context.SourceAbility = SourceAbility;
	Context.GameEffect = GameEffect;
	Context.HitIndex = TargetIndex;
	Context.Parameters = Parameters;
	return Context;
}
