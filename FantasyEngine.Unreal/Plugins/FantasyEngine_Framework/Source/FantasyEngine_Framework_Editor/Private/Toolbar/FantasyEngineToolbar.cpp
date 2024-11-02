#include "Toolbar/FantasyEngineToolbar.h"

#include "ISettingsModule.h"
#include "Toolbar/FantasyEngineStyle.h"
#include "Toolbar/FantasyEngineCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include "Toolbar/Actions/MenuAction_GenerateTables.h"

static const FName FoolishGameToolbarTabName("FantasyEngineToolbar");

#define LOCTEXT_NAMESPACE "Fantasy Engine Editor"

void FFantasyEngineToolbar::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FFantasyEngineStyle::Initialize();
	FFantasyEngineStyle::ReloadTextures();

	FFantasyEngineCommands::Register();

	CommandList = MakeShareable(new FUICommandList);

	CommandList->MapAction(
		FFantasyEngineCommands::Get().GenerateTables, FExecuteAction::CreateLambda([]
		{
			FMenuAction_GenerateTables().GenerateTables();
		}), FCanExecuteAction());
	CommandList->MapAction(
		FFantasyEngineCommands::Get().FrameworkSettings, FExecuteAction::CreateLambda([]
		{
			if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
			{
				SettingsModule->ShowViewer(TEXT("Project"), TEXT("Fantasy Engine"), TEXT("Fantasy Engine Settings"));
			}
		}), FCanExecuteAction());
	CommandList->MapAction(
		FFantasyEngineCommands::Get().FantasyEngineEditor, FExecuteAction::CreateLambda([]
		{
			if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
			{
				SettingsModule->ShowViewer(TEXT("Project"), TEXT("Fantasy Engine"),TEXT("Fantasy Engine Editor"));
			}
		}), FCanExecuteAction());

	// if (IPluginManager::Get().FindPlugin("WorldCreator"))
	// {
	CommandList->MapAction(
		FFantasyEngineCommands::Get().WorldCreatorSettings, FExecuteAction::CreateLambda([]
		{
			if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
			{
				SettingsModule->ShowViewer(TEXT("Project"), TEXT("Fantasy Engine"),TEXT("World Creator"));
			}
		}), FCanExecuteAction());

	CommandList->MapAction(
		FFantasyEngineCommands::Get().OpenPuzzleGeneratorWindow, FExecuteAction::CreateLambda([]
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FName("Puzzle Generator"));
		}), FCanExecuteAction());
	//}

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FFantasyEngineToolbar::RegisterMenus));
}

void FFantasyEngineToolbar::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FFantasyEngineStyle::Shutdown();

	FFantasyEngineCommands::Unregister();
}

TSharedRef<SWidget> FFantasyEngineToolbar::CreateMenu()
{
	const FFantasyEngineCommands& Commands = FFantasyEngineCommands::Get();
	FMenuBuilder MenuBuilder(true, CommandList);
	MenuBuilder.BeginSection(NAME_None, FText::FromString(TEXT("Table")));
	MenuBuilder.AddMenuEntry(Commands.GenerateTables,
	                         NAME_None,
	                         FText::FromString(TEXT("Generate")),
	                         FText::FromString(TEXT("Generate")),
	                         FSlateIcon(TEXT("FantasyEngineStyle"), TEXT("FantasyEngineEditor.GenerateTables"))
	);
	MenuBuilder.EndSection();
	if (IPluginManager::Get().FindPlugin("WorldCreator").IsValid())
	{
		//SubMenuBuilder.AddSeparator();
		MenuBuilder.BeginSection(NAME_None, FText::FromString(TEXT("World Creator")));
		MenuBuilder.AddMenuEntry(Commands.OpenPuzzleGeneratorWindow, NAME_None,
		                         FText::FromString(TEXT("Puzzle Generator")),
		                         FText::FromString(TEXT("Create Puzzles")),
		                         FSlateIcon(
			                         TEXT("WorldCreatorStyle"),
			                         TEXT("WorldCreatorEditor.ToolbarLogo"))
		);
		MenuBuilder.EndSection();
	}
	MenuBuilder.BeginSection(NAME_None, FText::FromString(TEXT("Help")));
	MenuBuilder.AddSubMenu(FText::FromString(TEXT("Settings")),
	                       FText::FromString(TEXT("Settings")),
	                       FNewMenuDelegate::CreateLambda([Commands](FMenuBuilder& SubMenuBuilder)
	                       {
		                       SubMenuBuilder.AddMenuEntry(Commands.FrameworkSettings, NAME_None,
		                                                   FText::FromString(TEXT("Settings")),
		                                                   FText::FromString(TEXT("Fantasy Engine Settings")),
		                                                   FSlateIcon(
			                                                   TEXT("FantasyEngineStyle"),
			                                                   TEXT("FantasyEngineEditor.FantasyEngineSettings"))
		                       );
		                       // TArray<TSharedRef<IPlugin>> Plugins = IPluginManager::Get().GetEnabledPlugins();
		                       // bool bWorldEditorExists = false;
		                       // for (const TSharedRef<IPlugin>& Plugin : Plugins)
		                       // {
		                       //  if (Plugin->GetName() == TEXT("WorldCreator"))
		                       //  {
		                       //   bWorldEditorExists = true;
		                       //  }
		                       // }
		                       if (IPluginManager::Get().FindPlugin("WorldCreator").IsValid())
		                       {
			                       //SubMenuBuilder.AddSeparator();
			                       SubMenuBuilder.AddMenuEntry(Commands.WorldCreatorSettings, NAME_None,
			                                                   FText::FromString(TEXT("World Creator")),
			                                                   FText::FromString(TEXT("World Creator Settings")),
			                                                   FSlateIcon(
				                                                   TEXT("WorldCreatorStyle"),
				                                                   TEXT("WorldCreatorEditor.ToolbarLogo"))
			                       );
		                       }
		                       SubMenuBuilder.AddSeparator();
		                       SubMenuBuilder.AddMenuEntry(Commands.FantasyEngineEditor, NAME_None,
		                                                   FText::FromString(TEXT("Editor")),
		                                                   FText::FromString(TEXT("Fantasy Engine Editor")),
		                                                   FSlateIcon(
			                                                   TEXT("FantasyEngineStyle"),
			                                                   TEXT("FantasyEngineEditor.FantasyEngineEditor"))
		                       );
	                       }),
	                       false,
	                       FSlateIcon(TEXT("FantasyEngineStyle"), TEXT("FantasyEngineEditor.Settings")));
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FFantasyEngineToolbar::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	// {
	// 	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.FoolishGame");
	// 	{
	// 		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
	// 		Section.AddMenuEntryWithCommandList(FFoolishGameCommands::Get().ToolbarAction, PluginCommands);
	// 	}
	// }

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Fantasy Engine");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitComboButton(
						"Fantasy Engine Editor",
						FUIAction(),
						FOnGetContent::CreateRaw(this, &FFantasyEngineToolbar::CreateMenu),
						FText::FromString(TEXT("Fantasy Engine")),
						FText::FromString(TEXT("Fantasy Engine")),
						FSlateIcon("FantasyEngineStyle", "FantasyEngineEditor.ToolbarLogo")
					));
				Entry.SetCommandList(CommandList);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
