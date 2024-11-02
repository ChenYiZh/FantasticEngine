// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "AudioSystem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UAudioClassConfigs : public UObject
{
	GENERATED_BODY()

public:
	// /**
	//  * 管理工具
	//  */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio System")
	// TObjectPtr<USoundMix> SM_Mix;
	/**
	 * 主音量控制
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio System")
	TObjectPtr<USoundClass> SC_Master;
	/**
	 * 音乐音量控制
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio System")
	TObjectPtr<USoundClass> SC_Music;
	/**
	 * 音效音量控制
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio System")
	TObjectPtr<USoundClass> SC_Effects;
	/**
	 * 对话音量控制
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio System")
	TObjectPtr<USoundClass> SC_Dialogue;
	/**
	 * 环境音量控制
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio System")
	TObjectPtr<USoundClass> SC_Ambient;
};

/**
 * 音效管理
 */
UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API UAudioSystem : public USystemBasis
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Sound|Settings")
	static FString GetSectionName()
	{
		return TEXT("Sound");
	}

private:
	/** 音量管理配置类 */
	UPROPERTY()
	UAudioClassConfigs* AudioClassConfigs = nullptr;

	/** 是否是静音 */
	UPROPERTY()
	bool bMute = false;

	/** 全局音量 */
	UPROPERTY()
	float MasterVolume = 1.f;

#pragma region 音乐管理
	/** 正在播放的背景音乐 */
	UPROPERTY()
	TObjectPtr<USoundBase> MusicPlaying;

	/** 下一首音乐 */
	UPROPERTY()
	TObjectPtr<USoundBase> NextMusic;

	/** 音乐音量 */
	UPROPERTY()
	float MusicVolume = 1.f;

	/** 音乐过渡时长 */
	UPROPERTY()
	float TotalMusicTransitionDuration = 0.f;

	/** 音乐过度剩余时长 */
	UPROPERTY()
	float MusicTransitionRemaining = -1.f;

	/** 是否在恢复音量的阶段 */
	UPROPERTY()
	bool bMusicRecoving = false;
#pragma  endregion

public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnTick_Implementation(float DeltaTime) override;

public:
	/** 设置音效的管理类 */
	UFUNCTION(BlueprintCallable, Category="Audio System")
	static USoundBase* SetSoundClass(USoundBase* Sound, USoundClass* Parent);

	/** 获取音频管理类 */
	UFUNCTION(BlueprintPure, Category="Audio System")
	static UAudioSystem* GetAudioSystem(const UObject* WorldContextObject);

#pragma region 获取管理

public:
	/** 获取音乐控制类 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", DisplayName="Music Class")
	USoundClass* GetMusicClass() const
	{
		return IsValid(AudioClassConfigs) && AudioClassConfigs->SC_Music
			       ? AudioClassConfigs->SC_Music.Get()
			       : nullptr;
	}

	/** 获取音效控制类 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", DisplayName="Effects Class")
	USoundClass* GetEffectsClass() const
	{
		return IsValid(AudioClassConfigs) && AudioClassConfigs->SC_Effects
			       ? AudioClassConfigs->SC_Effects.Get()
			       : nullptr;
	}

	/** 获取对话音效控制类 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", DisplayName="Dialogue Class")
	USoundClass* GetDialogueClass() const
	{
		return IsValid(AudioClassConfigs) && AudioClassConfigs->SC_Dialogue
			       ? AudioClassConfigs->SC_Dialogue.Get()
			       : nullptr;
	}

	/** 获取环境音效控制类 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", DisplayName="Ambient Class")
	USoundClass* GetAmbientClass() const
	{
		return IsValid(AudioClassConfigs) && AudioClassConfigs->SC_Ambient
			       ? AudioClassConfigs->SC_Ambient.Get()
			       : nullptr;
	}
#pragma endregion

#pragma region 播放控制

public:
	/** 设置播放背景音乐 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void PlayMusic(const UObject* WorldContextObject, USoundBase* Music,
	                      const float TransitionDuration = 0);

	/** 播放2D的音效 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void PlayEffectsSound2D(const UObject* WorldContextObject, USoundBase* EffectsSound);
#pragma endregion

#pragma region 音量控制

public:
	/** 设置是否全局静音 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void SetMute(const UObject* WorldContextObject, bool ToMute);

	/** 全局音量 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static bool IsMute(const UObject* WorldContextObject);

	/** 设置全局音量 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void SetMasterVolume(const UObject* WorldContextObject, float Volume);

	/** 全局音量 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static float GetMasterVolume(const UObject* WorldContextObject);

	/** 设置背景音乐音量 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void SetMusicVolume(const UObject* WorldContextObject, float Volume);

	/** 背景音乐音量 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static float GetMusicVolume(const UObject* WorldContextObject);

	/** 设置特效音量 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void SetEffectsVolume(const UObject* WorldContextObject, float Volume);

	/** 特效音量 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static float GetEffectsVolume(const UObject* WorldContextObject);

	/** 设置对话音量 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void SetDialogueVolume(const UObject* WorldContextObject, float Volume);

	/** 对话音量 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static float GetDialogueVolume(const UObject* WorldContextObject);

	/** 设置环境音量 */
	UFUNCTION(BlueprintCallable, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static void SetAmbientVolume(const UObject* WorldContextObject, float Volume);

	/** 环境音量 */
	UFUNCTION(BlueprintPure, Exec, Category="Audio System", meta=(WorldContext="WorldContextObject"))
	static float GetAmbientVolume(const UObject* WorldContextObject);
#pragma endregion
};
