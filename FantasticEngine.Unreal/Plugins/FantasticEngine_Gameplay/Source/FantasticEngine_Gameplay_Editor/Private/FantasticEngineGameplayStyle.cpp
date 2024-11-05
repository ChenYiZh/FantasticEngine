#include "FantasticEngineGameplayStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FFantasticEngineGameplayStyle::StyleInstance = nullptr;

void FFantasticEngineGameplayStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FFantasticEngineGameplayStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FFantasticEngineGameplayStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("FantasticEngineGameplayStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

TSharedRef<FSlateStyleSet> FFantasticEngineGameplayStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("FantasticEngine_Gameplay")->GetBaseDir() / TEXT("Resources"));
	
	Style->Set("FantasticEngineGameplayEditor.Strategy", new IMAGE_BRUSH(TEXT("icons/strategy"), Icon128x128));
	Style->Set("FantasticEngineGameplayEditor.Ability", new IMAGE_BRUSH(TEXT("icons/ability"), Icon128x128));
	Style->Set("FantasticEngineGameplayEditor.Attributes", new IMAGE_BRUSH(TEXT("icons/attributes"), Icon128x128));
	Style->Set("FantasticEngineGameplayEditor.GameEffect", new IMAGE_BRUSH(TEXT("icons/game_effect"), Icon128x128));


	return Style;
}

void FFantasticEngineGameplayStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FFantasticEngineGameplayStyle::Get()
{
	return *StyleInstance;
}
