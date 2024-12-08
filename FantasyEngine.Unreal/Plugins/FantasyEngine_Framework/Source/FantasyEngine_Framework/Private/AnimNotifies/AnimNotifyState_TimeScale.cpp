// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_TimeScale.h"

// UAnimNotifyState_TimeScale::UAnimNotifyState_TimeScale()
// {
// 	Scale = 1.f;
// #if WITH_EDITORONLY_DATA
// 	NotifyColor = FColor::Orange;
// #endif
// }

void UAnimNotifyState_TimeScale::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//InScale = Animation->RateScale;
	//MeshComp->GlobalAnimRateScale = Animation->RateScale * Scale;
	if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
	{
		MeshComp->GetAnimInstance()->Montage_SetPlayRate(Montage, Scale);
	}
	else
	{
		MeshComp->SetPlayRate(Scale);
	}
}

void UAnimNotifyState_TimeScale::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	//MeshComp->GlobalAnimRateScale = 1.f;
	if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
	{
		MeshComp->GetAnimInstance()->Montage_SetPlayRate(Montage, 1);
	}
	else
	{
		MeshComp->SetPlayRate(1);
	}
}
