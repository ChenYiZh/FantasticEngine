// Fill out your copyright notice in the Description page of Project Settings.
#include "Toolbar/FantasticEngineCommands.h"
#define LOCTEXT_NAMESPACE "Fantastic Engine Editor"

void FFantasticEngineCommands::RegisterCommands()
{
	UI_COMMAND(ToolbarAction, "Fantastic Engine", "Fantastic Engine functional actions.", EUserInterfaceActionType::Button,
	           FInputChord());
	UI_COMMAND(GenerateTables, "Generate Tables", "Generate tables from setting path.",
	           EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(FrameworkSettings, "Fantastic Engine Settings", "Modify the Fantastic Engine settings.",
	           EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(FantasticEngineEditor, "Fantastic Engine Editor", "Config the Fantastic Engine settings.",
	           EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(WorldCreatorSettings, "World Creator Settings", "Modify the world creator settings.",
	           EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(OpenPuzzleGeneratorWindow, "Puzzle Creator", "Create puzzle groups", EUserInterfaceActionType::Button,
	           FInputChord());
}
#undef LOCTEXT_NAMESPACE
