// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_PlaySound.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/UObjectToken.h"

FString UAnimNotifyState_PlaySound::GetNotifyName_Implementation() const
{
	if (Sound)
	{
		return Sound->GetName();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}

#if WITH_EDITOR
void UAnimNotifyState_PlaySound::ValidateAssociatedAssets()
{
	static const FName NAME_AssetCheck("AssetCheck");

	if (Sound != nullptr && !Sound->IsOneShot())
	{
		UObject* ContainingAsset = GetContainingAsset();

		FMessageLog AssetCheckLog(NAME_AssetCheck);

		const FText MessageLooping = FText::Format(
			NSLOCTEXT("AnimNotify", "Sound_ShouldNotLoop",
			          "Sound {0} used in anim notify for asset {1} is set to looping, but the slot is a one-shot (it won't be played to avoid leaking an instance per notify)."),
			FText::AsCultureInvariant(Sound->GetPathName()),
			FText::AsCultureInvariant(ContainingAsset->GetPathName()));
		AssetCheckLog.Warning()
		             ->AddToken(FUObjectToken::Create(ContainingAsset))
		             ->AddToken(FTextToken::Create(MessageLooping));

		if (GIsEditor)
		{
			AssetCheckLog.Notify(MessageLooping, EMessageSeverity::Warning, /*bForce=*/ true);
		}
	}
}
#endif

void UAnimNotifyState_PlaySound::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	// Don't call super to avoid call back in to blueprints
	if (Sound && MeshComp)
	{
		if (!Sound->IsOneShot())
		{
			UE_LOG(LogAudio, Warning,
			       TEXT(
				       "PlaySound notify: Anim %s tried to play a sound asset which is not a one-shot: '%s'. Spawning suppressed."
			       ), *GetNameSafe(Animation), *GetNameSafe(Sound));
			return;
		}

		// #if WITH_EDITORONLY_DATA
		// 		UWorld* World = MeshComp->GetWorld();
		// 		if (World && World->WorldType == EWorldType::EditorPreview)
		// 		{
		// 			UGameplayStatics::PlaySound2D(World, Sound, VolumeMultiplier, PitchMultiplier);
		// 		}
		// 		else
		// #endif
		{
			// if (bFollow)
			// {
			AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, AttachName, FVector(ForceInit),
			                                                      EAttachLocation::SnapToTarget, false,
			                                                      VolumeMultiplier, PitchMultiplier);
			// }
			// else
			// {
			// 	UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
			// }
		}
	}
}

void UAnimNotifyState_PlaySound::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
}
