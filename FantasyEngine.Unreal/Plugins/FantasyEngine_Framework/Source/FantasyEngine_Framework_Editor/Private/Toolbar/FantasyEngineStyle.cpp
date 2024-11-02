#include "Toolbar/FantasyEngineStyle.h"
#include "FantasyEngine_Framework_Editor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FFantasyEngineStyle::StyleInstance = nullptr;

void FFantasyEngineStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FFantasyEngineStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FFantasyEngineStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("FantasyEngineStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

TSharedRef<FSlateStyleSet> FFantasyEngineStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("FantasyEngine_Framework")->GetBaseDir() / TEXT("Resources"));

	Style->Set("FantasyEngineEditor.ToolbarLogo", new IMAGE_BRUSH(TEXT("Icon128"), Icon20x20));
	Style->Set("FantasyEngineEditor.GenerateTables", new IMAGE_BRUSH(TEXT("icons/generate_tables"), Icon16x16));

	Style->Set("FantasyEngineEditor.Settings", new IMAGE_BRUSH(TEXT("icons/settings"), Icon16x16));
	Style->Set("FantasyEngineEditor.FantasyEngineSettings", new IMAGE_BRUSH(TEXT("icons/framework_settings"), Icon16x16));
	Style->Set("FantasyEngineEditor.FantasyEngineEditor", new IMAGE_BRUSH(TEXT("icons/fantasy_engine_editor"), Icon16x16));


	Style->Set("FantasyEngineEditor.GameRoot", new IMAGE_BRUSH(TEXT("icons/game_root"), Icon128x128));
	Style->Set("FantasyEngineEditor.BlueprintError", new IMAGE_BRUSH(TEXT("icons/blueprint_error"), Icon128x128));
	Style->Set("FantasyEngineEditor.GameInstance", new IMAGE_BRUSH(TEXT("icons/game_instance"), Icon128x128));
	Style->Set("FantasyEngineEditor.GameDefines", new IMAGE_BRUSH(TEXT("icons/game_defines"), Icon128x128));
	Style->Set("FantasyEngineEditor.GameAction", new IMAGE_BRUSH(TEXT("icons/game_action"), Icon128x128));
	Style->Set("FantasyEngineEditor.GameEvent", new IMAGE_BRUSH(TEXT("icons/game_event"), Icon128x128));
	Style->Set("FantasyEngineEditor.GameSystem", new IMAGE_BRUSH(TEXT("icons/game_system"), Icon128x128));
	Style->Set("FantasyEngineEditor.GameScene", new IMAGE_BRUSH(TEXT("icons/game_scene"), Icon128x128));


	return Style;
}

void FFantasyEngineStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FFantasyEngineStyle::Get()
{
	return *StyleInstance;
}
