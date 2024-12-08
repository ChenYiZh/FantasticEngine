// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenComponent.h"

#include "Components/Tween/TweenPlayerComponent.h"

UTweenComponent::UTweenComponent()
{
	// Tween 动画需要开启时间控件
	PrimaryComponentTick.bCanEverTick = true;
	bHiddenInGame = false;
}

void UTweenComponent::BeginPlay()
{
	Super::BeginPlay();
	// 开始时获取一下可以操作的控件
	FindComponent();
	// 重置一下状态
	if (bPlayOnEnable)
	{
		ResetToBegin();
	}
	CheckComponentState();
}


void UTweenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float DeltaSeconds = Speed * DeltaTime;
	if (!bIsPlaying) { return; }
	if (RemainingUntilStart > 0)
	{
		RemainingUntilStart -= DeltaSeconds;
		return;
	}
	Remaining -= DeltaSeconds;
	bool bValid = true;
	float Alpha = Remaining / Duration;
	// 播放次数 > 1 时重新计算比例
	if (Time > 1)
	{
		const int32 RemainingTime = FMath::Floor(Remaining / SingleDuration); //剩余需要完成的次数
		const float SingleRemaining = Remaining - RemainingTime * SingleDuration;
		const float SingleDurationWithoutInterval = SingleDuration - Interval;

		if (SingleRemaining > SingleDurationWithoutInterval)
		{
			bValid = false;
		}
		else
		{
			Alpha = SingleRemaining / SingleDurationWithoutInterval;
		}
	}

	if (bValid)
	{
		Alpha = FMath::Clamp(bPlayingBack ? Alpha : 1.f - Alpha, 0.f, 1.f);
		if (FloatCurve)
		{
			Alpha = FloatCurve->GetFloatValue(Alpha);
		}
		Play(Alpha);
	}
	if (Remaining < 0)
	{
		OnceFinished();
	}
}

void UTweenComponent::Play(float Alpha)
{
	if (!IsValid(ControlledComponent)) { return; }
	Play(ControlledComponent, Alpha);
	OnPlay.Broadcast(ControlledComponent, Alpha);
	K2_OnPlay.Broadcast(ControlledComponent, Alpha);
}

void UTweenComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
}

void UTweenComponent::PlayForward_Implementation()
{
	if (!bInitialized)
	{
		ResetToBegin();
	}
	if (bPlayingBack)
	{
		Remaining = Duration - Remaining;
	}
	SingleDuration = (Duration - (Time - 1) * Interval) / Time + Interval;
	bPlayingBack = false;
	bIsPlaying = true;
}

void UTweenComponent::PlayBack_Implementation()
{
	if (!bInitialized)
	{
		ResetToEnd();
	}
	if (!bPlayingBack)
	{
		Remaining = Duration - Remaining;
	}
	SingleDuration = (Duration - (Time - 1) * Interval) / Time + Interval;
	bPlayingBack = true;
	bIsPlaying = true;
}

void UTweenComponent::ResetToBegin_Implementation()
{
	Remaining = Duration;
	RemainingUntilStart = StartDelay;
	bPlayingBack = false;
	bInitialized = true;
	Play(0);
}

void UTweenComponent::ResetToEnd_Implementation()
{
	Remaining = Duration;
	RemainingUntilStart = StartDelay;
	bPlayingBack = true;
	bInitialized = true;
	Play(1);
}

void UTweenComponent::Pause_Implementation()
{
	bIsPlaying = false;
}

void UTweenComponent::Resume_Implementation()
{
	bIsPlaying = true;
}

void UTweenComponent::Stop_Implementation()
{
	bIsPlaying = false;
	CallOnFinished();
}

void UTweenComponent::FindComponent_Implementation()
{
	USceneComponent* RootComponent = GetOwner()->GetRootComponent();
	if (this == RootComponent)
	{
		ControlledComponent = this;
		return;
	}
	ControlledComponent = GetAttachParent();
	while (ControlledComponent != RootComponent && ControlledComponent.IsA<UTweenPlayerComponent>())
	{
		ControlledComponent = ControlledComponent->GetAttachParent();
	}
	if (ControlledComponent.IsA<UTweenPlayerComponent>())
	{
		ControlledComponent = this;
	}
}

void UTweenComponent::OnceFinished_Implementation()
{
	bIsPlaying = false;
	switch (PlayStyle)
	{
	case ETweenPlayStyle::Once:
		{
			CallOnFinished();
		}
		break;
	case ETweenPlayStyle::Loop:
		{
			if (bPlayingBack)
			{
				ResetToEnd();
				PlayBack();
			}
			else
			{
				ResetToBegin();
				PlayForward();
			}
		}
		break;
	case ETweenPlayStyle::PingPong:
		{
			if (bPlayingBack)
			{
				ResetToBegin();
				PlayForward();
			}
			else
			{
				ResetToEnd();
				PlayBack();
			}
		}
		break;
	}
}

void UTweenComponent::CheckComponentState_Implementation()
{
	const bool bEnabled = IsVisible() && ControlledComponent->IsVisible();
	if (bLastEnabled != bEnabled)
	{
		if (bEnabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}
}

void UTweenComponent::OnEnable_Implementation()
{
	bLastEnabled = true;
	if (bPlayOnEnable)
	{
		if (!bInitialized)
		{
			if (bPlayingBack)
			{
				ResetToEnd();
				PlayBack();
			}
			else
			{
				ResetToBegin();
				PlayForward();
			}
		}
		else
		{
			Resume();
		}
	}
}

void UTweenComponent::OnDisable_Implementation()
{
	bLastEnabled = false;
	if (bResetOnDisable)
	{
		Stop();
	}
	else
	{
		Pause();
	}
}
