// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Internationalization/Culture.h"
#include "EnumDefines.generated.h"

UENUM(BlueprintType)
enum class EPlatform:uint8
{
	/**
	 * 桌面平台，Windows，Linux，Mac，Xbox
	 */
	Desktop = 0,
	/**
	 * 手机平台，IOS，Android
	 */
	Mobile = 10,
	/**
	 * 主机平台，PS，Switch
	 */
	GameConsole = 20,
	/**
	 * HoloLens
	 */
	HoloLens = 30
};

/**
 * 屏幕适配方案
 */
UENUM(BlueprintType)
enum class EScreenAdapter:uint8
{
	/**
	 * 自动适配
	 */
	Auto = 0,
	/**
	 * 只适配刘海屏
	 */
	HairScreen = 10,
	/**
	 * 只适配带鱼屏
	 */
	WideScreen = 20,
	/**
	 * 不适配 
	 */
	None = 100,
};

/**
 * 鼠标类型
 */
UENUM(BlueprintType)
enum class EMouseType:uint8
{
	/**
	 * 跟从GameDefine的鼠标类型
	 */
	Default = 0,
	/**
	 * 显示
	 */
	Visible = 10,
	/**
	 * 隐藏
	 */
	Hide = 20
};

/** 语言枚举 **/
UENUM(BlueprintType)
namespace ELanguage
{
	enum Type:int32
	{
		/**
		 * 简体中文
		 */
		ChineseSimplified = 0 UMETA(DisplayName="简体中文"),
		/**
		 * 繁体中文
		 */
		ChineseTraditional = 1 UMETA(DisplayName="繁體中文"),
		/**
		 * 英文
		 */
		English = 5 UMETA(DisplayName="English"),
		/**
		 * 日语
		 */
		Japanese = 10 UMETA(DisplayName="日本語")
	};
}
