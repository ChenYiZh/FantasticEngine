// Fill out your copyright notice in the Description page of Project Settings.


#include "Toolbar/Actions/MenuAction_GenerateTables.h"

#include "AssetToolsModule.h"
#include "AssetViewUtils.h"
#include "BlueprintCompilationManager.h"
#include "BlueprintEditor.h"
#include "DataTableEditorUtils.h"
#include "FileHelpers.h"
#include "FantasyEngineEditorSettings.h"
#include "JsonObjectConverter.h"
#include "PackageHelperFunctions.h"
#include "PackageSourceControlHelper.h"
#include "AssetTools/Private/AssetTools.h"
#include "Basis/SystemBasis.h"
#include "Defines/Array2.h"
#include "Factories/BlueprintFactory.h"
#include "Factories/DataTableFactory.h"
#include "Factories/StructureFactory.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Misc/FileHelper.h"
#include "UserDefinedStructure/UserDefinedStructEditorData.h"

void FMenuAction_GenerateTables::GenerateTables()
{
	if (!MakeTables()) return;
	TArray<FTableInfo> Tables;
	if (!LoadJson(Tables) || Tables.Num() == 0) return;

	const UFantasyEngineEditorSettings* Settings = GetDefault<UFantasyEngineEditorSettings>();

	TableStructPath = Settings->DataTableStructFolder.Path;
	TablePath = Settings->DataTableFolder.Path;
	TableStructHeader = Settings->DataTableStructHeader;
	TableHeader = Settings->DataTableHeader;

	TArray<UPackage*> PackagesToCreate;
	TArray<UPackage*> PackagesToSave;
	TMap<FName, UDataTable*> DataTables;
	for (FTableInfo Table : Tables)
	{
		bool bTableStructExists;
		if (UUserDefinedStruct* TableStruct = CheckTableStructure(Table, bTableStructExists))
		{
			if (bTableStructExists)
			{
				PackagesToSave.Add(TableStruct->GetPackage());
			}
			else
			{
				PackagesToCreate.Add(TableStruct->GetPackage());
			}
			//TableStructs.Push(FAssetData(TableStruct));
			bool bDataTableExists;
			if (UDataTable* DataTable = CheckTable(Table, TableStruct, bDataTableExists))
			{
				if (bDataTableExists)
				{
					PackagesToSave.Add(DataTable->GetPackage());
				}
				else
				{
					PackagesToCreate.Add(DataTable->GetPackage());
				}
				DataTables.Add(DataTable->GetFName(), DataTable);
			}
		}
	}

	// FPackageSourceControlHelper PackageHelper;
	// PackageHelper.Delete(PackagesToSave);

	// for (UPackage* PackageToSave : PackagesToSave)
	// {
	// 	FString FilePath = USourceControlHelpers::PackageFilename(PackageToSave->GetName());
	// 	IPlatformFile::GetPlatformPhysical().SetReadOnly(*FilePath, false);
	// 	IPlatformFile::GetPlatformPhysical().DeleteFile(*FilePath);
	// }

	FEditorFileUtils::PromptToCheckoutPackages(false, PackagesToCreate, nullptr, nullptr, false, false);
	for (UPackage* Package : PackagesToSave)
	{
		SavePackageHelper(Package, Package->GetLoadedPath().GetLocalFullPath());
		//GEditor->SavePackage(Package,Package,Package->GetLoadedPath(),)
	}
	//FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false, nullptr);
	//SavePackageHelper
	//FEditorFileUtils::SaveDirtyPackages

	// bool bOutAnyPackagesUnloaded;
	// FText OutErrorMessage;
	//UEditorLoadingAndSavingUtils::UnloadPackages(PackagesToSave, bOutAnyPackagesUnloaded, OutErrorMessage);
	//UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, false);

	// FEditorFileUtils::SaveDirtyContentPackages(
	// 	TArray<UClass*>{UUserDefinedStruct::StaticClass(), UDataTable::StaticClass()}, false, true, true, true);

	// bool bNotifyNoPackagesSaved;
	// FEditorFileUtils::SaveDirtyPackages(false, false, true, true, false, true, &bNotifyNoPackagesSaved,
	//                                     [PackagesToSave](const UPackage* Package)
	//                                     {
	// 	                                    return PackagesToSave.Contains(Package);
	//                                     });


	CreateBlueprintFunction(DataTables);

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Tables has generated")));
}

bool FMenuAction_GenerateTables::MakeTables()
{
	const UFantasyEngineEditorSettings* Settings = GetDefault<UFantasyEngineEditorSettings>();
	if (!Settings)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Settings load failed.")));
		return false;
	}
	RootPath = FPaths::Combine(FPaths::ProjectDir(), Settings->ExcelTablesFolder.Path);
	if (!FPaths::DirectoryExists(RootPath))
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(
			                     FString::Printf(TEXT("The Excel path: '%s' is not exists."), ToCStr(RootPath))));
		return false;
	}
	ExeFilePath = FPaths::Combine(FPaths::ProjectDir(), Settings->ConverterFilePath.FilePath);
	if (!FPaths::FileExists(ExeFilePath))
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(
			                     FString::Printf(TEXT("The bat path: '%s' is not exists."), ToCStr(ExeFilePath))));
		return false;
	}
	ExeFolder = FPaths::GetPath(ExeFilePath);
	FString ExeFilename = FPaths::GetCleanFilename(ExeFilePath);
	int32 OutReturnCode;
	FString OutStdOut;
	FString OutStdErr;
	//设置成UTF-8
	//FPlatformProcess::ExecProcess(TEXT("CHCP 65001"), nullptr, &OutReturnCode, &OutStdOut, &OutStdErr);
	FPlatformProcess::ExecProcess(ToCStr(ExeFilePath), ToCStr(Settings->ConverterParams),
	                              &OutReturnCode, &OutStdOut, &OutStdErr, *ExeFolder);
	TArray<FString> Logs;
	bool bSuccess = OutReturnCode == 0;
	if (!bSuccess)
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(
			                     TEXT("return: ") + FString::FromInt(OutReturnCode) + TEXT("\n") + OutStdErr));
		OutStdErr.ParseIntoArray(Logs,LINE_TERMINATOR, true);
		Logs.Push(TEXT("return: ") + FString::FromInt(OutReturnCode));
	}
	else
	{
		OutStdOut.ParseIntoArray(Logs,LINE_TERMINATOR, true);
	}
	// TArray<uint8> Buffer;
	// UStringConverter::ToUTF8(OutStdOut, Buffer);
	// OutStdOut = ANSI_TO_TCHAR((ANSICHAR*)Buffer.GetData());
	for (const FString& Line : Logs)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), ToCStr(Line));
	}
	return bSuccess;
}

bool FMenuAction_GenerateTables::LoadJson(TArray<FTableInfo>& Tables)
{
	JsonFilePath = FPaths::Combine(ExeFolder,TEXT("../Client/tables.json"));
	FString Json;
	if (!FFileHelper::LoadFileToString(Json, ToCStr(JsonFilePath)))
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(
			                     FString::Printf(TEXT("The file: %s is not exists."), ToCStr(JsonFilePath))));
		return false;
	}
	return FJsonObjectConverter::JsonArrayStringToUStruct(Json, &Tables);
	//return FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Tables);
}

UUserDefinedStruct* FMenuAction_GenerateTables::CheckTableStructure(const FTableInfo& Table, bool& bExists) const
{
	const FName AssetName = FName(TableStructHeader.ToString() + Table.Name.ToString());
	const FString PackageName = FString::Printf(TEXT("%s/%s"), ToCStr(TableStructPath), ToCStr(AssetName.ToString()));

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	FSoftObjectPath SoftPath(FName(PackageName), AssetName, FString());
	// TArray<UObject*> OutObjects;
	// AssetTools.FindSoftReferencesToObject(SoftPath, OutObjects);
	//UUserDefinedStruct* TableStruct = OutObjects.Num() > 0 ? Cast<UUserDefinedStruct>(OutObjects[0]) : nullptr;
	//UUserDefinedStruct* TableStruct = TSoftObjectPtr<UUserDefinedStruct>(SoftPath).Get();
	UUserDefinedStruct* TableStruct = LoadObject<UUserDefinedStruct>(nullptr, ToCStr(SoftPath.ToString()));
	//UUserDefinedStruct* TableStruct = Cast<UUserDefinedStruct>(SoftPath.ResolveObject());

	bExists = TableStruct != nullptr;
	if (bExists)
	{
		//UEditorLoadingAndSavingUtils::UnloadPackages

		// UObject* NewTableStruct = AssetTools.DuplicateAsset(AssetName.ToString(), TableStructPath, TableStruct);
		// FPackageSourceControlHelper PackageHelper;
		// PackageHelper.Delete(TableStruct->GetPackage());
		// TableStruct = Cast<UUserDefinedStruct>(NewTableStruct);
	}
	else
	{
		TableStruct = Cast<UUserDefinedStruct>(AssetTools.CreateAsset(
			AssetName.ToString(), TableStructPath,
			UUserDefinedStruct::StaticClass(),
			NewObject<UStructureFactory>()));
	}
	if (!TableStruct)
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(TEXT("UUserDefinedStruct create failed.")));
		return nullptr;
	}

	FStructureEditorUtils::ChangeTooltip(TableStruct, Table.Name.ToString());

	TArray<FStructVariableDescription>& TableVars = FStructureEditorUtils::GetVarDesc(TableStruct);
	TArray<FStructVariableDescription> OldVars(TableVars);
	TArray<FGuid> GuidsToRemove;
	for (const FStructVariableDescription& TableVar : TableVars)
	{
		GuidsToRemove.Push(TableVar.VarGuid);
	}
	bool bSuccess = true;
	FGuid LastVarGuid;
	for (int32 i = 0; i < Table.Headers.Num(); i++)
	{
		const FHeaderInfo& Header = Table.Headers[i];
		FStructVariableDescription* Variable = OldVars.FindByPredicate(
			[Header](const FStructVariableDescription& OldVar)
			{
				return OldVar.FriendlyName == Header.Name.ToString();
			});
		FGuid VarGuid;
		if (Variable)
		{
			VarGuid = Variable->VarGuid;
			GuidsToRemove.Remove(VarGuid);
			FStructureEditorUtils::ChangeVariableType(TableStruct, VarGuid, GetFieldType(Header.Type));
		}
		else
		{
			TArray UnmodifiedVars(FStructureEditorUtils::GetVarDesc(TableStruct));
			if (FStructureEditorUtils::AddVariable(TableStruct, GetFieldType(Header.Type)))
			{
				// 查找新建的字段
				const TArray<FStructVariableDescription>& NewVars = FStructureEditorUtils::GetVarDesc(TableStruct);
				for (const FStructVariableDescription& NewVar : NewVars)
				{
					if (!UnmodifiedVars.ContainsByPredicate([NewVar](const FStructVariableDescription& UnmodifiedVar)
					{
						return NewVar.VarGuid == UnmodifiedVar.VarGuid;
					}))
					{
						VarGuid = NewVar.VarGuid;
						break;
					}
				}
			}
			else
			{
				bSuccess = false;
				break;
			}
		}

		FStructureEditorUtils::RenameVariable(TableStruct, VarGuid, Header.Name.ToString());
		FStructureEditorUtils::ChangeMultiLineTextEnabled(TableStruct, VarGuid, Header.Type == EHeaderType::STRING);
		FStructureEditorUtils::ChangeVariableTooltip(TableStruct, VarGuid, Header.Description);

		if (i == 0)
		{
			FGuid Guid = OldVars[0].VarGuid;
			FStructureEditorUtils::MoveVariable(TableStruct, VarGuid, Guid, FStructureEditorUtils::PositionAbove);
		}
		else
		{
			FStructureEditorUtils::MoveVariable(TableStruct, VarGuid, LastVarGuid,
			                                    FStructureEditorUtils::PositionBelow);
		}
		LastVarGuid = VarGuid;
	}

	if (!bSuccess)
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(TEXT("UUserDefinedStruct create failed.")));
		return nullptr;
	}

	for (const FGuid& GuidToRemove : GuidsToRemove)
	{
		FStructureEditorUtils::RemoveVariable(TableStruct, GuidToRemove);
	}

	//FStructureEditorUtils::ChangeSaveGameEnabled(TableStruct, LastVarGuid, true);

	// FSavePackageArgs SaveArgs;
	// SaveArgs.TopLevelFlags = RF_Standalone;
	//
	// // Rehydrate the payloads so that we move the virtual payloads also. Add the auto save flag to notify the editor extensions that they shouldn't prompt the user or generate/update some asset on save.
	// SaveArgs.SaveFlags |= SAVE_RehydratePayloads | SAVE_FromAutosave;
	// GEditor->Save(TableStruct->GetPackage(), TableStruct, ToCStr(TableStruct->GetName()), SaveArgs);
	return TableStruct;
}

UDataTable* FMenuAction_GenerateTables::CheckTable(const FTableInfo& TableInfo, UUserDefinedStruct* TableStruct,
                                                   bool& bExists)
{
	const FName AssetName = FName(TableHeader.ToString() + TableInfo.Name.ToString());
	const FString PackageName = FString::Printf(TEXT("%s/%s"), ToCStr(TablePath), ToCStr(AssetName.ToString()));

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	FSoftObjectPath SoftPath(FName(PackageName), AssetName, FString());
	//TArray<UObject*> OutObjects;
	//AssetTools.FindSoftReferencesToObject(SoftPath, OutObjects);
	//UDataTable* Table = OutObjects.Num() > 0 ? Cast<UDataTable>(OutObjects[0]) : nullptr;
	//UDataTable* Table = TSoftObjectPtr<UDataTable>(SoftPath).Get();
	UDataTable* Table = LoadObject<UDataTable>(nullptr, ToCStr(SoftPath.ToString()));
	//UDataTable* Table = Cast<UDataTable>(SoftPath.ResolveObject());
	bExists = Table != nullptr;
	if (bExists)
	{
		// UObject* NewTable = AssetTools.DuplicateAsset(AssetName.ToString(), TablePath, Table);
		// FPackageSourceControlHelper PackageHelper;
		// PackageHelper.Delete(Table->GetPackage());
		// Table = Cast<UDataTable>(NewTable);
		Table->RowStruct = TableStruct;
	}
	else
	{
		UDataTableFactory* Factory = NewObject<UDataTableFactory>();
		Factory->Struct = TableStruct;
		Table = Cast<UDataTable>(AssetTools.CreateAsset(
			AssetName.ToString(), TablePath,
			UDataTable::StaticClass(), Factory));
	}

	if (!Table)
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(TEXT("DataTable create failed.")));
		return nullptr;
	}

	FString TableFilePath = FPaths::Combine(ExeFolder,TEXT("../Client/Tables/"))
		+ TableInfo.Name.ToString() + TEXT(".txt");

	TArray<FString> Lines;
	FFileHelper::LoadFileToStringArray(Lines, ToCStr(TableFilePath));

	FString Json;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Json);
	JsonWriter->WriteArrayStart();
	if (Lines.Num() > 0)
	{
		for (const FString& Line : Lines)
		{
			if (Line.IsEmpty()) { continue; }
			TArray<FString> Values;
			Line.ParseIntoArray(Values,TEXT("\t"), false);
			JsonWriter->WriteObjectStart();

			JsonWriter->WriteValue(TEXT("Name"), FName(Values[0]).ToString());
			for (int32 i = 0; i < TableInfo.Headers.Num(); i++)
			{
				const FHeaderInfo& Header = TableInfo.Headers[i];
				ConvertJson(JsonWriter, Header.Type, ToCStr(Header.Name.ToString()), Values[i]);
			}
			JsonWriter->WriteObjectEnd();
			// uint8* Row = FDataTableEditorUtils::AddRow(Table, FName(Values[0]));
			// DataTableUtils::AssignStringToProperty()
		}
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->Close();

	TArray<FStructVariableDescription>& TableVars = FStructureEditorUtils::GetVarDesc(TableStruct);
	for (FStructVariableDescription& Var : TableVars)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *Var.FriendlyName);
	}

	UDataTableFunctionLibrary::FillDataTableFromJSONString(Table, Json);
	// FSavePackageArgs SaveArgs;
	// SaveArgs.TopLevelFlags = RF_Standalone;
	//
	// // Rehydrate the payloads so that we move the virtual payloads also. Add the auto save flag to notify the editor extensions that they shouldn't prompt the user or generate/update some asset on save.
	// SaveArgs.SaveFlags |= SAVE_RehydratePayloads | SAVE_FromAutosave;
	//
	// GEditor->Save(Table->GetPackage(), Table, ToCStr(Table->GetName()), SaveArgs);
	return Table;
}

FEdGraphPinType FMenuAction_GenerateTables::GetFieldType(const EHeaderType& FieldType)
{
	FEdGraphPinType GraphPinType;
	GraphPinType.ContainerType = EPinContainerType::None;
	GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
	GraphPinType.PinSubCategory = NAME_None;
	GraphPinType.PinSubCategoryObject = nullptr;
	switch (FieldType)
	{
	case EHeaderType::STRING: break;
	case EHeaderType::SHORT:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::INT:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::LONG:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int64;
		break;
	case EHeaderType::USHORT:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::UINT:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::ULONG:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int64;
		break;
	case EHeaderType::FLOAT:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Real;
		GraphPinType.PinSubCategory = UEdGraphSchema_K2::PC_Float;
		break;
	case EHeaderType::DOUBLE:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Real;
		GraphPinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
		break;
	case EHeaderType::BOOL:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Boolean;
		break;
	case EHeaderType::BYTE:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Byte;
		break;
	case EHeaderType::CHAR:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		break;
	case EHeaderType::DATE:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		GraphPinType.PinSubCategory = TEXT("datetime");
		break;
	case EHeaderType::TIME:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		GraphPinType.PinSubCategory = TEXT("datetime");
		break;
	case EHeaderType::DATETIME:
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		GraphPinType.PinSubCategory = TEXT("datetime");
		break;

	case EHeaderType::ARRAY_STRING:
		GraphPinType.ContainerType = EPinContainerType::Array;
		break;
	case EHeaderType::ARRAY_SHORT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::ARRAY_INT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::ARRAY_LONG:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int64;
		break;
	case EHeaderType::ARRAY_USHORT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::ARRAY_UINT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
		break;
	case EHeaderType::ARRAY_ULONG:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Int64;
		break;
	case EHeaderType::ARRAY_FLOAT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Float;
		break;
	case EHeaderType::ARRAY_DOUBLE:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Double;
		break;
	case EHeaderType::ARRAY_BOOL:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Boolean;
		break;
	case EHeaderType::ARRAY_BYTE:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Byte;
		break;
	case EHeaderType::ARRAY_CHAR:
		//GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		break;
	case EHeaderType::ARRAY_DATE:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		GraphPinType.PinSubCategory = TEXT("datetime");
		break;
	case EHeaderType::ARRAY_TIME:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		GraphPinType.PinSubCategory = TEXT("datetime");
		break;
	case EHeaderType::ARRAY_DATETIME:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
		GraphPinType.PinSubCategory = TEXT("datetime");
		break;

	case EHeaderType::ARRAY_ARRAY_STRING:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FStringArray::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_SHORT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FInt32Array::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_INT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FInt32Array::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_LONG:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FInt64Array::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_USHORT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FInt32Array::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_UINT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FInt32Array::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_ULONG:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FInt64Array::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_FLOAT:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FFloatArray::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_DOUBLE:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FDoubleArray::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_BOOL:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FBoolArray::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_BYTE:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FUInt8Array::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_CHAR:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_String;
	//GraphPinType.PinSubCategoryObject = FStringArray::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_DATE:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FDateTimeArray::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_TIME:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FDateTimeArray::StaticStruct();
		break;
	case EHeaderType::ARRAY_ARRAY_DATETIME:
		GraphPinType.ContainerType = EPinContainerType::Array;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		GraphPinType.PinSubCategoryObject = FDateTimeArray::StaticStruct();
		break;
	}

	return GraphPinType;
}

void FMenuAction_GenerateTables::ConvertJson(
	const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>>& JsonWriter,
	const EHeaderType& FieldType, const TCHAR* Key, const FString& Value)
{
	switch (FieldType)
	{
	case EHeaderType::STRING:
		JsonWriter->WriteValue<FString>(Key, Value);
		break;
	case EHeaderType::SHORT:
		JsonWriter->WriteValue<int32>(Key, FCString::Atoi(ToCStr(Value)));
		break;
	case EHeaderType::INT:
		JsonWriter->WriteValue<int32>(Key, FCString::Atoi(ToCStr(Value)));
		break;
	case EHeaderType::LONG:
		JsonWriter->WriteValue<int64>(Key, FCString::Atoi64(ToCStr(Value)));
		break;
	case EHeaderType::USHORT:
		JsonWriter->WriteValue<int32>(Key, FCString::Atoi(ToCStr(Value)));
		break;
	case EHeaderType::UINT:
		JsonWriter->WriteValue<int32>(Key, FCString::Atoi(ToCStr(Value)));
		break;
	case EHeaderType::ULONG:
		JsonWriter->WriteValue<int64>(Key, FCString::Atoi64(ToCStr(Value)));
		break;
	case EHeaderType::FLOAT:
		JsonWriter->WriteValue<float>(Key, FCString::Atof(ToCStr(Value)));
		break;
	case EHeaderType::DOUBLE:
		JsonWriter->WriteValue<double>(Key, FCString::Atod(ToCStr(Value)));
		break;
	case EHeaderType::BYTE:
		JsonWriter->WriteValue<uint8>(Key, FCString::Atoi(ToCStr(Value)));
		break;
	case EHeaderType::CHAR:
		JsonWriter->WriteValue<FString>(Key, Value);
		break;
	case EHeaderType::DATE:
	case EHeaderType::TIME:
	case EHeaderType::DATETIME:
		{
			// FDateTime DateTime;
			// FDateTime::Parse(Value, DateTime);
			// JsonWriter->WriteValue<FDateTime>(Key, DateTime);
			JsonWriter->WriteValue(Key, Value);
		}
		break;
	case EHeaderType::BOOL:
		JsonWriter->WriteValue(Key, Value == TEXT("1") || Value.Equals(TEXT("true"), ESearchCase::IgnoreCase));
		break;


	case EHeaderType::ARRAY_STRING:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(Values[i]);
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_SHORT:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_INT:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_LONG:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atoi64(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_USHORT:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_UINT:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ULONG:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atoi64(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_FLOAT:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atof(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_DOUBLE:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atod(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_BOOL:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(
					Values[i] == TEXT("1") || Values[i].Equals(TEXT("true"), ESearchCase::IgnoreCase));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_BYTE:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[i])));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_CHAR:
		JsonWriter->WriteValue(Key, Value.Replace(TEXT(","),TEXT("")));
		break;
	case EHeaderType::ARRAY_DATE:
	case EHeaderType::ARRAY_TIME:
	case EHeaderType::ARRAY_DATETIME:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(","), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(Values[i]);
			}
			JsonWriter->WriteArrayEnd();
		}
		break;

	case EHeaderType::ARRAY_ARRAY_STRING:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(Values[k]);
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_SHORT:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_INT:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_LONG:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atoi64(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_USHORT:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_UINT:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_ULONG:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atoi64(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_FLOAT:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atof(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_DOUBLE:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atod(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_BOOL:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(
						Values[i] == TEXT("1") || Values[i].Equals(TEXT("true"), ESearchCase::IgnoreCase));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_BYTE:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(FCString::Atoi(ToCStr(Values[k])));
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_CHAR:
		{
			TArray<FString> Values;
			Value.ParseIntoArray(Values,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int i = 0; i < Values.Num(); i++)
			{
				JsonWriter->WriteValue(Values[i].Replace(TEXT(","),TEXT("")));
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	case EHeaderType::ARRAY_ARRAY_DATE:
	case EHeaderType::ARRAY_ARRAY_TIME:
	case EHeaderType::ARRAY_ARRAY_DATETIME:
		{
			TArray<FString> Lines;
			Value.ParseIntoArray(Lines,TEXT(";"), false);
			JsonWriter->WriteArrayStart(Key);
			for (int32 i = 0; i < Lines.Num(); i++)
			{
				TArray<FString> Values;
				Lines[i].ParseIntoArray(Values,TEXT(","), false);
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteArrayStart(TEXT("Array"));
				for (int k = 0; k < Values.Num(); k++)
				{
					JsonWriter->WriteValue(Values[k]);
				}
				JsonWriter->WriteArrayEnd();
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayEnd();
		}
		break;
	}
}

void FMenuAction_GenerateTables::CreateBlueprintFunction(const TMap<FName, UDataTable*>& Tables)
{
	const UFantasyEngineEditorSettings* Settings = GetDefault<UFantasyEngineEditorSettings>();
	if (!Settings)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Settings load failed.")));
		return;
	}
	FString Path = Settings->DataTableSystemFolder.Path;
	FName Filename = TEXT("BP_DataTableSystem");

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	FSoftObjectPath SoftPath(FName(Path + TEXT("/") + Filename.ToString()), FName(Filename.ToString()), // + TEXT("_C")
	                         FString());

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, ToCStr(SoftPath.ToString()));
	bool bExists = Blueprint != nullptr;
	if (bExists)
	{
		// FPackageSourceControlHelper PackageHelper;
		// PackageHelper.Delete(Blueprint->GetPackage());
	}
	else
	{
		UBlueprintFactory* Factory = NewObject<UBlueprintFactory>();
		Factory->ParentClass = USystemBasis::StaticClass();
		Blueprint = Cast<UBlueprint>(AssetTools.CreateAsset(
			Filename.ToString(), Path,
			UBlueprint::StaticClass(), Factory));
	}

	if (!Blueprint)
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(Filename.ToString() + TEXT(" create failed.")));
		return;
	}

	TSet<FName> BlueprintVariables;
	FBlueprintEditorUtils::GetClassVariableList(Blueprint, BlueprintVariables);
	TSet<FName> VariablesToRemove(BlueprintVariables);

	for (const TPair<FName, UDataTable*>& Table : Tables)
	{
		FName VariableName = Table.Key; //Table.Value->GetFName();
		FEdGraphPinType GraphPinType;
		GraphPinType.ContainerType = EPinContainerType::None;
		GraphPinType.PinCategory = UEdGraphSchema_K2::PC_Object;
		GraphPinType.PinSubCategory = NAME_None;
		GraphPinType.PinSubCategoryObject = UDataTable::StaticClass();
		//GraphPinType.PinSubCategoryMemberReference.MemberParent = Table.Value;
		if (BlueprintVariables.Contains(VariableName))
		{
			VariablesToRemove.Remove(VariableName);
			FBlueprintEditorUtils::ChangeMemberVariableType(Blueprint, VariableName, GraphPinType);
		}
		else
		{
			FBlueprintEditorUtils::AddMemberVariable(Blueprint, VariableName, GraphPinType);
		}
		FBlueprintEditorUtils::SetBlueprintPropertyReadOnlyFlag(Blueprint, VariableName, true);
		// FBPVariableDescription& VarDesc = Blueprint->NewVariables[0];
		// bool bExists = false;
		// for (FBPVariableDescription& NewVar : Blueprint->NewVariables)
		// {
		// 	if (NewVar.FriendlyName == VariableName.ToString())
		// 	{
		// 		VarDesc = NewVar;
		// 		bExists = true;
		// 		break;
		// 	}
		// }
		// FString PackageName = Table.Value->GetFullName();
		// VarDesc.DefaultValue = PackageName;
		// //FBlueprintEditorUtils::FixupVariableDescription(Blueprint, VarDesc);
	}

	for (const FName& VarName : VariablesToRemove)
	{
		FBlueprintEditorUtils::RemoveMemberVariable(Blueprint, VarName);
	}

	FBPCompileRequest Request(Blueprint, EBlueprintCompileOptions::None, nullptr);
	FBlueprintCompilationManager::CompileSynchronously(Request);

	FBlueprintEditorUtils::RefreshVariables(Blueprint);

	for (FBPVariableDescription& NewVar : Blueprint->NewVariables)
	{
		FName NewVarName = NewVar.VarName; // FName(NewVar.FriendlyName);
		if (Tables.Contains(NewVarName))
		{
			FString TablePath = Tables[NewVarName]->GetPathName();
			NewVar.DefaultValue = TablePath;
		}
	}

	FBlueprintEditorUtils::RefreshVariables(Blueprint);
	FBlueprintCompilationManager::CompileSynchronously(Request);
	if (bExists)
	{
		SavePackageHelper(Blueprint->GetPackage(), Blueprint->GetPackage()->GetLoadedPath().GetLocalFullPath());
	}
	else
	{
		FEditorFileUtils::PromptForCheckoutAndSave(TArray{Blueprint->GetPackage()}, false, false, nullptr);
	}

	FText OutErrorMsg;
	TArray<UPackage*> Packages;
	Packages.Push(Blueprint->GetPackage());
	UPackageTools::ReloadPackages(Packages, OutErrorMsg, EReloadPackagesInteractionMode::Interactive);
}
