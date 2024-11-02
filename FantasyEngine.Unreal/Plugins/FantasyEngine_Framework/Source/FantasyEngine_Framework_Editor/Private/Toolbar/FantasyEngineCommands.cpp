// Fill out your copyright notice in the Description page of Project Settings.
#include "Toolbar/FantasyEngineCommands.h"
#define LOCTEXT_NAMESPACE "Fantasy Engine Editor"

void FFantasyEngineCommands::RegisterCommands()
{
	UI_COMMAND(ToolbarAction, "Fantasy Engine", "Fantasy Engine functional actions.", EUserInterfaceActionType::Button,
	           FInputChord());
	UI_COMMAND(GenerateTables, "Generate Tables", "Generate tables from setting path.",
	           EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(FrameworkSettings, "Fantasy Engine Settings", "Modify the Fantasy Engine settings.",
	           EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(FantasyEngineEditor, "Fantasy Engine Editor", "Config the Fantasy Engine settings.",
	           EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(WorldCreatorSettings, "World Creator Settings", "Modify the world creator settings.",
	           EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(OpenPuzzleGeneratorWindow, "Puzzle Creator", "Create puzzle groups", EUserInterfaceActionType::Button,
	           FInputChord());
}
#undef LOCTEXT_NAMESPACE
