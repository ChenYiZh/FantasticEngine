// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_TimeScaleCurve.h"

#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_TimeScaleCurve::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Seconds = 0;
	//InScale = Animation->RateScale;
	if (Curve)
	{
		//Log(UKismetSystemLibrary::GetDisplayName(MeshComp->GetOwner()));
		TotalSeconds = TotalDuration;
		//Animation->RateScale = Curve->GetFloatValue(0);
		if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
		{
			MeshComp->GetAnimInstance()->Montage_SetPlayRate(Montage, Curve->GetFloatValue(0));
		}
		else
		{
			MeshComp->SetPlayRate(Curve->GetFloatValue(0));
		}
	}
}

void UAnimNotifyState_TimeScaleCurve::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (Curve)
	{
		//Seconds += FrameDeltaTime * Animation->RateScale;
		//Animation->RateScale = Curve->GetFloatValue(FMath::Clamp(Seconds / TotalSeconds, 0.f, 1.f));
		if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
		{
			Seconds += FrameDeltaTime * MeshComp->GetAnimInstance()->Montage_GetPlayRate(Montage);
			MeshComp->GetAnimInstance()->Montage_SetPlayRate(
				Montage, Curve->GetFloatValue(FMath::Clamp(Seconds / TotalSeconds, 0.f, 1.f)));
		}
		else
		{
			Seconds += FrameDeltaTime * MeshComp->GetPlayRate();
			MeshComp->SetPlayRate(Curve->GetFloatValue(FMath::Clamp(Seconds / TotalSeconds, 0.f, 1.f)));
		}
	}
}

void UAnimNotifyState_TimeScaleCurve::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	//Animation->RateScale = InScale;
	//MeshComp->GlobalAnimRateScale = 1;
	if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
	{
		MeshComp->GetAnimInstance()->Montage_SetPlayRate(Montage, 1);
	}
	else
	{
		MeshComp->SetPlayRate(Curve->GetFloatValue(1));
	}
}
