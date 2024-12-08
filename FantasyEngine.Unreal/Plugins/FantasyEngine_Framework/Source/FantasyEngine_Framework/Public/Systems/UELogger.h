// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "Log/ILogger.h"
#include "UObject/Object.h"
#include "UELogger.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Console, Log, All);

USTRUCT()
struct FLogMessage
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName Level;
	UPROPERTY()
	FString Message;
	UPROPERTY()
	bool AddToScreen;
	UPROPERTY()
	float TimeToDisplay;

	// FLogMessage(const FName& InLevel, const FString& InMessage, const bool& InAddToScreen,
	//             const float& InTimeToDisplay):
	// 	Level(InLevel), Message(InMessage), AddToScreen(InAddToScreen), TimeToDisplay(InTimeToDisplay)
	// {
	// }
};

/**
 * 
 */
UCLASS()
class FANTASYENGINE_FRAMEWORK_API UUELogger : public USystemBasis, public IILogger
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<FLogMessage> Messages;

	FCriticalSection Mutex;

public:	
	virtual bool EnableTickBeforeInitialized_Implementation() override;

	virtual void OnTick_Implementation(float DeltaTime) override;

	virtual void SaveLog_Implementation(const FName& Level, const FString& Message, const bool AddToScreen,
	                                    const float TimeToDisplay) override;

private:
	void LogMessage(const FLogMessage& Message);

	const FColor& GetLevelColor(const FName& Level) const;

	void LogToConsole(const FName& Level, const FString& Message);
};
