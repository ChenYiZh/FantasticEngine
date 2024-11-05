#include "Toolbar/FantasticEngineStyle.h"
#include "FantasticEngine_Framework_Editor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FFantasticEngineStyle::StyleInstance = nullptr;

void FFantasticEngineStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FFantasticEngineStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FFantasticEngineStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("FantasticEngineStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

TSharedRef<FSlateStyleSet> FFantasticEngineStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("FantasticEngine_Framework")->GetBaseDir() / TEXT("Resources"));

	Style->Set("FantasticEngineEditor.ToolbarLogo", new IMAGE_BRUSH(TEXT("Icon128"), Icon20x20));
	Style->Set("FantasticEngineEditor.GenerateTables", new IMAGE_BRUSH(TEXT("icons/generate_tables"), Icon16x16));

	Style->Set("FantasticEngineEditor.Settings", new IMAGE_BRUSH(TEXT("icons/settings"), Icon16x16));
	Style->Set("FantasticEngineEditor.FantasticEngineSettings", new IMAGE_BRUSH(TEXT("icons/framework_settings"), Icon16x16));
	Style->Set("FantasticEngineEditor.FantasticEngineEditor", new IMAGE_BRUSH(TEXT("icons/fantastic_engine_editor"), Icon16x16));


	Style->Set("FantasticEngineEditor.GameRoot", new IMAGE_BRUSH(TEXT("icons/game_root"), Icon128x128));
	Style->Set("FantasticEngineEditor.BlueprintError", new IMAGE_BRUSH(TEXT("icons/blueprint_error"), Icon128x128));
	Style->Set("FantasticEngineEditor.GameInstance", new IMAGE_BRUSH(TEXT("icons/game_instance"), Icon128x128));
	Style->Set("FantasticEngineEditor.GameDefines", new IMAGE_BRUSH(TEXT("icons/game_defines"), Icon128x128));
	Style->Set("FantasticEngineEditor.GameAction", new IMAGE_BRUSH(TEXT("icons/game_action"), Icon128x128));
	Style->Set("FantasticEngineEditor.GameEvent", new IMAGE_BRUSH(TEXT("icons/game_event"), Icon128x128));
	Style->Set("FantasticEngineEditor.GameSystem", new IMAGE_BRUSH(TEXT("icons/game_system"), Icon128x128));
	Style->Set("FantasticEngineEditor.GameScene", new IMAGE_BRUSH(TEXT("icons/game_scene"), Icon128x128));


	return Style;
}

void FFantasticEngineStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FFantasticEngineStyle::Get()
{
	return *StyleInstance;
}
