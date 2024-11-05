// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/UELogger.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Log/FEConsole.h"

DEFINE_LOG_CATEGORY(Console);

bool UUELogger::EnableTickBeforeInitialized_Implementation()
{
	return true;
}

void UUELogger::OnTick_Implementation(float DeltaTime)
{
	FScopeLock SetLock(&Mutex);
	for (int i = 0; i < Messages.Num(); i++)
	{
		const FLogMessage& Message = Messages[i];
		LogMessage(Message);
	}
	Messages.Empty();
}

void UUELogger::SaveLog_Implementation(const FName& Level, const FString& Message, const bool AddToScreen,
                                       const float TimeToDisplay)
{
	FLogMessage Logger{Level, Message, AddToScreen, TimeToDisplay};
	if (IsInGameThread())
	{
		LogMessage(Logger);
	}
	else
	{
		FScopeLock SetLock(&Mutex);
		Messages.Push(Logger);
	}
}

void UUELogger::LogMessage(const FLogMessage& Message)
{
	if (Message.AddToScreen && Message.TimeToDisplay > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, Message.TimeToDisplay, GetLevelColor(Message.Level),
		                                 ToCStr(Message.Message));
	}
	LogToConsole(Message.Level, Message.Message);
}

const FColor& UUELogger::GetLevelColor(const FName& Level) const
{
	if (Level == ULogLevel::LOG_ERROR)
	{
		return FColor::Red;
	}
	if (Level == ULogLevel::LOG_WARN)
	{
		return FColor::Orange;
	}
	if (Level == ULogLevel::LOG_INFO)
	{
		return FColor::Cyan;
	}
	return FColor::White;
}

void UUELogger::LogToConsole(const FName& Level, const FString& Message)
{
	if (Level == ULogLevel::LOG_ERROR)
	{
		UE_LOG(Console, Error, TEXT("%s"), ToCStr(Message));
	}
	else if (Level == ULogLevel::LOG_WARN)
	{
		UE_LOG(Console, Warning, TEXT("%s"), ToCStr(Message));
	}
	else
	{
		UE_LOG(Console, Display, TEXT("%s"), ToCStr(Message));
	}
}
