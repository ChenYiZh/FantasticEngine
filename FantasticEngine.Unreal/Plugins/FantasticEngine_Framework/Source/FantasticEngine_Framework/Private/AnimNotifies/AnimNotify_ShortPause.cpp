// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_ShortPause.h"

#include "Kismet/KismetSystemLibrary.h"

void UAnimNotify_ShortPause::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (UWorld* World = GetWorld())
	{
		OriginRateScale = Animation->RateScale;
		Animation->RateScale = 0; //0.000001f;
		PlayingAnimation = Animation;
		World->GetTimerManager().SetTimer(TImerHandle, this,
		                                  &UAnimNotify_ShortPause::OnDelayCallback, Duration);
	}
	//UKismetSystemLibrary::Delay(GetWorld(), Duration,
	//FLatentActionInfo(INDEX_NONE, INDEX_NONE,TEXT("OnDelayCallback"), this));
}

void UAnimNotify_ShortPause::OnDelayCallback()
{
	PlayingAnimation->RateScale = OriginRateScale;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TImerHandle);
	}
}
