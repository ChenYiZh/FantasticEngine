#include "Toolbar/FantasticEngineToolbar.h"

#include "ISettingsModule.h"
#include "Toolbar/FantasticEngineStyle.h"
#include "Toolbar/FantasticEngineCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include "Toolbar/Actions/MenuAction_GenerateTables.h"

static const FName FantasticEngineToolbarTabName("FantasticEngineToolbar");

#define LOCTEXT_NAMESPACE "Fantastic Engine Editor"

void FFantasticEngineToolbar::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FFantasticEngineStyle::Initialize();
	FFantasticEngineStyle::ReloadTextures();

	FFantasticEngineCommands::Register();

	CommandList = MakeShareable(new FUICommandList);

	CommandList->MapAction(
		FFantasticEngineCommands::Get().GenerateTables, FExecuteAction::CreateLambda([]
		{
			FMenuAction_GenerateTables().GenerateTables();
		}), FCanExecuteAction());
	CommandList->MapAction(
		FFantasticEngineCommands::Get().FrameworkSettings, FExecuteAction::CreateLambda([]
		{
			if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
			{
				SettingsModule->ShowViewer(TEXT("Project"), TEXT("Fantastic Engine"), TEXT("Fantastic Engine Settings"));
			}
		}), FCanExecuteAction());
	CommandList->MapAction(
		FFantasticEngineCommands::Get().FantasticEngineEditor, FExecuteAction::CreateLambda([]
		{
			if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
			{
				SettingsModule->ShowViewer(TEXT("Project"), TEXT("Fantastic Engine"),TEXT("Fantastic Engine Editor"));
			}
		}), FCanExecuteAction());

	// if (IPluginManager::Get().FindPlugin("WorldCreator"))
	// {
	CommandList->MapAction(
		FFantasticEngineCommands::Get().WorldCreatorSettings, FExecuteAction::CreateLambda([]
		{
			if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
			{
				SettingsModule->ShowViewer(TEXT("Project"), TEXT("Fantastic Engine"),TEXT("World Creator"));
			}
		}), FCanExecuteAction());

	CommandList->MapAction(
		FFantasticEngineCommands::Get().OpenPuzzleGeneratorWindow, FExecuteAction::CreateLambda([]
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FName("Puzzle Generator"));
		}), FCanExecuteAction());
	//}

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FFantasticEngineToolbar::RegisterMenus));
}

void FFantasticEngineToolbar::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FFantasticEngineStyle::Shutdown();

	FFantasticEngineCommands::Unregister();
}

TSharedRef<SWidget> FFantasticEngineToolbar::CreateMenu()
{
	const FFantasticEngineCommands& Commands = FFantasticEngineCommands::Get();
	FMenuBuilder MenuBuilder(true, CommandList);
	MenuBuilder.BeginSection(NAME_None, FText::FromString(TEXT("Table")));
	MenuBuilder.AddMenuEntry(Commands.GenerateTables,
	                         NAME_None,
	                         FText::FromString(TEXT("Generate")),
	                         FText::FromString(TEXT("Generate")),
	                         FSlateIcon(TEXT("FantasticEngineStyle"), TEXT("FantasticEngineEditor.GenerateTables"))
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
		                                                   FText::FromString(TEXT("Fantastic Engine Settings")),
		                                                   FSlateIcon(
			                                                   TEXT("FantasticEngineStyle"),
			                                                   TEXT("FantasticEngineEditor.FantasticEngineSettings"))
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
		                       SubMenuBuilder.AddMenuEntry(Commands.FantasticEngineEditor, NAME_None,
		                                                   FText::FromString(TEXT("Editor")),
		                                                   FText::FromString(TEXT("Fantastic Engine Editor")),
		                                                   FSlateIcon(
			                                                   TEXT("FantasticEngineStyle"),
			                                                   TEXT("FantasticEngineEditor.FantasticEngineEditor"))
		                       );
	                       }),
	                       false,
	                       FSlateIcon(TEXT("FantasticEngineStyle"), TEXT("FantasticEngineEditor.Settings")));
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FFantasticEngineToolbar::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	// {
	// 	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.FantasticEngine");
	// 	{
	// 		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
	// 		Section.AddMenuEntryWithCommandList(FFantasticEngineCommands::Get().ToolbarAction, PluginCommands);
	// 	}
	// }

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Fantastic Engine");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitComboButton(
						"Fantastic Engine Editor",
						FUIAction(),
						FOnGetContent::CreateRaw(this, &FFantasticEngineToolbar::CreateMenu),
						FText::FromString(TEXT("Fantastic Engine")),
						FText::FromString(TEXT("Fantastic Engine")),
						FSlateIcon("FantasticEngineStyle", "FantasticEngineEditor.ToolbarLogo")
					));
				Entry.SetCommandList(CommandList);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
