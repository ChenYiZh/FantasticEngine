// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/AudioSystem.h"

#include "AudioDevice.h"
#include "GameRoot.h"
#include "Defines/Settings.h"
#include "Kismet/GameplayStatics.h"

void UAudioSystem::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
	if (UGameDefines* Defines = UGameRoot::GetGameDefines(this))
	{
		if (const TSubclassOf<UAudioClassConfigs> ConfigClass = Defines->GetAudioClassConfigs())
		{
			AudioClassConfigs = NewObject<UAudioClassConfigs>(this, ConfigClass);
		}
	}

	if (!AudioClassConfigs)
	{
		AudioClassConfigs = NewObject<UAudioClassConfigs>(this);
	}

	bMute = USettings::GetBool(GetSectionName(),TEXT("Mute"), false);
	MasterVolume = USettings::GetFloat(GetSectionName(),TEXT("MasterVolume"), 1.f);
	const float SettingsMusicVolume = USettings::GetFloat(GetSectionName(),TEXT("MusicVolume"), 1.f);
	const float EffectsVolume = USettings::GetFloat(GetSectionName(),TEXT("EffectsVolume"), 1.f);
	const float DialogueVolume = USettings::GetFloat(GetSectionName(),TEXT("DialogueVolume"), 1.f);
	const float AmbientVolume = USettings::GetFloat(GetSectionName(),TEXT("AmbientVolume"), 1.f);
	SetMute(this, bMute);
	SetMasterVolume(this, MasterVolume);
	SetMusicVolume(this, SettingsMusicVolume);
	SetEffectsVolume(this, EffectsVolume);
	SetDialogueVolume(this, DialogueVolume);
	SetAmbientVolume(this, AmbientVolume);
}

void UAudioSystem::OnTick_Implementation(float DeltaTime)
{
	Super::OnTick_Implementation(DeltaTime);
	if (MusicTransitionRemaining < 0 && !FMath::IsNearlyZero(MusicTransitionRemaining)) { return; }
	// 判断是不是私服务器这些
	if (!IsValid(GetAudioSystem(this))) { return; }

	MusicTransitionRemaining -= DeltaTime;
	bool bNextMusic = !IsValid(MusicPlaying);
	if (!bNextMusic)
	{
		bNextMusic = FMath::IsNearlyZero(TotalMusicTransitionDuration);
	}
	if (!bNextMusic)
	{
		const float CurrentMusicVolumeStep = FMath::Clamp(MusicTransitionRemaining / TotalMusicTransitionDuration,
		                                                  0.f, 1.f);
		const float Volume = FMath::Lerp(0.f, MusicVolume,
		                                 CurrentMusicVolumeStep);
		GetMusicClass()->Properties.Volume = bMusicRecoving ? 1 - Volume : Volume;
		bNextMusic = FMath::IsNearlyZero(CurrentMusicVolumeStep);
	}
	if (bNextMusic)
	{
		if (bMusicRecoving)
		{
			if (IsValid(MusicPlaying))
			{
				GetMusicClass()->Properties.Volume = MusicVolume;
				MusicTransitionRemaining = -1.f;
			}
		}
		else
		{
			if (IsValid(MusicPlaying))
			{
				if (const FAudioDeviceHandle AudioDevice = GetWorld()->GetAudioDevice())
				{
					USoundBase* CurrentMusic = MusicPlaying;
					FAudioThread::RunCommandOnAudioThread([AudioDevice,CurrentMusic]
					{
						const TArray<FActiveSound*>& ActiveSounds = AudioDevice.GetAudioDevice()->GetActiveSounds();
						for (FActiveSound* ActiveSound : ActiveSounds)
						{
							if (ActiveSound->GetSound() == CurrentMusic)
							{
								//AudioDevice.GetAudioDevice()->StopActiveSound(ActiveSound);
								ActiveSound->Stop();
								break;
							}
						}
					});
				}
				MusicPlaying = nullptr;
			}

			MusicPlaying = SetSoundClass(NextMusic, GetMusicClass());
			NextMusic = nullptr;
			bMusicRecoving = true;
			if (IsValid(MusicPlaying))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), MusicPlaying);
			}
			MusicTransitionRemaining = TotalMusicTransitionDuration;
			//GetMusicClass()->Properties.Volume = 0.f;
		}
	}
}

USoundBase* UAudioSystem::SetSoundClass(USoundBase* Sound, USoundClass* Parent)
{
	if (IsValid(Sound))
	{
		USoundBase* SoundBase = Sound;
		if (Parent)
		{
			SoundBase->SoundClassObject = Parent;
		}
		return SoundBase;
	}
	return nullptr;
}

UAudioSystem* UAudioSystem::GetAudioSystem(const UObject* WorldContextObject)
{
	if (!UGameRoot::GetRoot(WorldContextObject)) { return nullptr; }
	const UWorld* ThisWorld = GEngine->GetWorldFromContextObject(WorldContextObject,
	                                                             EGetWorldErrorMode::LogAndReturnNull);
	if (!ThisWorld || !ThisWorld->bAllowAudioPlayback || ThisWorld->IsNetMode(NM_DedicatedServer))
	{
		return nullptr;
	}
	UAudioSystem* System = Singleton<UAudioSystem>(WorldContextObject);
	return System;
}

void UAudioSystem::PlayMusic(const UObject* WorldContextObject, USoundBase* Music,
                             const float TransitionDuration)
{
	UAudioSystem* System = GetAudioSystem(WorldContextObject);
	if (!System) { return; }
	// if (Music)
	// {
	System->TotalMusicTransitionDuration = IsValid(System->MusicPlaying)
		                                       ? TransitionDuration / 2.f
		                                       : TransitionDuration;
	System->MusicTransitionRemaining = System->TotalMusicTransitionDuration;
	System->NextMusic = Music;
	System->bMusicRecoving = false;
	//return;
	//}

	// System->TotalMusicTransitionDuration = TransitionDuration;
	// System->MusicTransitionRemaining = System->TotalMusicTransitionDuration;
	// System->NextMusic = nullptr;
	// System->MusicVolumeOnChangeMusic = 0;
	// System->bMusicRecoving = false;
}

void UAudioSystem::PlayEffectsSound2D(const UObject* WorldContextObject, USoundBase* EffectsSound)
{
	if (UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (EffectsSound)
		{
			UGameplayStatics::PlaySound2D(WorldContextObject, SetSoundClass(EffectsSound, System->GetEffectsClass()));
		}
	}
}

void UAudioSystem::SetMute(const UObject* WorldContextObject, bool ToMute)
{
	USettings::SetBool(GetSectionName(),TEXT("Mute"), ToMute);
	if (UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		System->bMute = ToMute;
		if (System->AudioClassConfigs->SC_Master)
		{
			System->AudioClassConfigs->SC_Master->Properties.Volume = ToMute ? 0.f : System->MasterVolume;
		}
	}
}

bool UAudioSystem::IsMute(const UObject* WorldContextObject)
{
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		return System->bMute;
	}
	return false;
}

void UAudioSystem::SetMasterVolume(const UObject* WorldContextObject, float Volume)
{
	USettings::SetFloat(GetSectionName(),TEXT("MasterVolume"), Volume);
	if (UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		System->MasterVolume = Volume;
		if (!System->bMute && System->AudioClassConfigs->SC_Master)
		{
			System->AudioClassConfigs->SC_Master->Properties.Volume = Volume;
		}
	}
}

float UAudioSystem::GetMasterVolume(const UObject* WorldContextObject)
{
	if (UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Master)
		{
			return System->AudioClassConfigs->SC_Master->Properties.Volume;
		}
	}
	return 0.f;
}

void UAudioSystem::SetMusicVolume(const UObject* WorldContextObject, float Volume)
{
	USettings::SetFloat(GetSectionName(),TEXT("MusicVolume"), Volume);
	if (UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		System->MusicVolume = Volume;
		if (System->AudioClassConfigs->SC_Music)
		{
			System->AudioClassConfigs->SC_Music->Properties.Volume = Volume;
		}
	}
}

float UAudioSystem::GetMusicVolume(const UObject* WorldContextObject)
{
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Music)
		{
			return System->AudioClassConfigs->SC_Music->Properties.Volume;
		}
	}
	return 0.f;
}

void UAudioSystem::SetEffectsVolume(const UObject* WorldContextObject, float Volume)
{
	USettings::SetFloat(GetSectionName(),TEXT("EffectsVolume"), Volume);
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Effects)
		{
			System->AudioClassConfigs->SC_Effects->Properties.Volume = Volume;
		}
	}
}

float UAudioSystem::GetEffectsVolume(const UObject* WorldContextObject)
{
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Effects)
		{
			return System->AudioClassConfigs->SC_Effects->Properties.Volume;
		}
	}
	return 0.f;
}

void UAudioSystem::SetDialogueVolume(const UObject* WorldContextObject, float Volume)
{
	USettings::SetFloat(GetSectionName(),TEXT("DialogueVolume"), Volume);
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Dialogue)
		{
			System->AudioClassConfigs->SC_Dialogue->Properties.Volume = Volume;
		}
	}
}

float UAudioSystem::GetDialogueVolume(const UObject* WorldContextObject)
{
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Dialogue)
		{
			return System->AudioClassConfigs->SC_Dialogue->Properties.Volume;
		}
	}
	return 0.f;
}

void UAudioSystem::SetAmbientVolume(const UObject* WorldContextObject, float Volume)
{
	USettings::SetFloat(GetSectionName(),TEXT("AmbientVolume"), Volume);
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Ambient)
		{
			System->AudioClassConfigs->SC_Ambient->Properties.Volume = Volume;
		}
	}
}

float UAudioSystem::GetAmbientVolume(const UObject* WorldContextObject)
{
	if (const UAudioSystem* System = GetAudioSystem(WorldContextObject))
	{
		if (System->AudioClassConfigs->SC_Ambient)
		{
			return System->AudioClassConfigs->SC_Ambient->Properties.Volume;
		}
	}
	return 0.f;
}
