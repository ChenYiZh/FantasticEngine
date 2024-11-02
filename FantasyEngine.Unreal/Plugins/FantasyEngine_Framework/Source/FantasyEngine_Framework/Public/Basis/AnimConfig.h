#pragma once
#include "AnimConfig.generated.h"

UENUM(BlueprintType)
enum class EUIAnimType : uint8
{
	None = 0,
	Opacity = 1,
	Scale = 2,
	Round = 3,
	Custom = 4
};

USTRUCT(BlueprintType)
struct FANTASYENGINE_FRAMEWORK_API FAnimConfig
{
	GENERATED_BODY()
public:
	/**
	 * 显示时动画类型
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EUIAnimType AnimationIn = EUIAnimType::Scale;
	/**
	 * 显示时动画时长
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AnimationInSeconds = 0.3f;
	/**
	 * 消失时动画类型
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EUIAnimType AnimationOut = EUIAnimType::Opacity;
	/**
	 * 消失时动画时长
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AnimationOutSeconds = 0.3f;
};
