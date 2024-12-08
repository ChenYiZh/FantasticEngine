// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_PlayStepEffect.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Log/FEConsole.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void UAnimNotify_PlayStepEffect_Handle::OnStep_Implementation(USkeletalMeshComponent* MeshComp, bool IsLeft,
                                                              EPhysicalSurface SurfaceType)
{
	const UEnum* TypeEnum = FindObject<UEnum>(nullptr, TEXT("EPhysicalSurface"));
	FString Name = TypeEnum->GetNameStringByIndex(static_cast<int32>(SurfaceType));
	UFEConsole::Write(
		MeshComp->GetFName().ToString() + TEXT(" ") + Name + TEXT(": ") + (IsLeft ? TEXT("Left") : TEXT("Right")));
}

void UAnimNotify_PlayStepEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Actor = MeshComp->GetOwner();
	if (Actor == nullptr) { return; }
	FVector StartPosition = Actor->GetActorLocation();
	FVector EndPosition = StartPosition + FVector::DownVector * 200;
	TArray<AActor*> IgnoreActors;
	FHitResult Result;
	if (!UKismetSystemLibrary::LineTraceSingle(Actor, StartPosition, EndPosition, ETraceTypeQuery::TraceTypeQuery1,
	                                           false,
	                                           IgnoreActors, EDrawDebugTrace::None, Result, true))
	{
		return;
	}
	if (!Result.PhysMaterial.IsValid()) { return; }
	if (!IsValid(Handle) && HandleClass != nullptr)
	{
		Handle = NewObject<UAnimNotify_PlayStepEffect_Handle>(this, HandleClass);
	}
	if (IsValid(Handle))
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Result.PhysMaterial.Get());
		Handle->OnStep(MeshComp, IsLeftFoot, SurfaceType);
	}
}
