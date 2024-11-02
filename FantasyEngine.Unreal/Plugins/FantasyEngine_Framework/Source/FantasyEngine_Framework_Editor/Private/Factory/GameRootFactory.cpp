// Fill out your copyright notice in the Description page of Project Settings.
#if 0

#include "Factory/GameRootFactory.h"

#include "ClassViewerModule.h"
#include "GameRoot.h"
#include "ClassViewer/Private/UnloadedBlueprintData.h"
#include "Kismet2/SClassPickerDialog.h"

#define LOCTEXT_NAMESPACE "UGameRootFactory"

class FGameRootClassFilter : public IClassViewerFilter
{
public:
	TSet<const UClass*> AllowedChildrenOfClasses;

	EClassFlags DisallowedClassFlags;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
	                            TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
	                                    const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
	                                    TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) !=
			EFilterReturn::Failed;
	}
};


UGameRootFactory::UGameRootFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UGameRoot::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UGameRootFactory::ConfigureProperties()
{
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	FClassViewerInitializationOptions Options;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.bShowNoneOption = false;
	Options.bExpandAllNodes = true;

	TSharedPtr<FGameRootClassFilter> Filter = MakeShareable(new FGameRootClassFilter);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Options.ExtraPickerCommonClasses.Add(UGameRoot::StaticClass());

	Filter->DisallowedClassFlags = CLASS_Deprecated | CLASS_NewerVersionExists;
	Filter->AllowedChildrenOfClasses.Add(UGameRoot::StaticClass());

	const FText TitleText = LOCTEXT("Create Game Root", "Pick Parent Class for New Game Root");

	UClass* ChosenParentClass = nullptr;
	bool isSuccessful =
		SClassPickerDialog::PickClass(TitleText, Options, ChosenParentClass, UGameRoot::StaticClass());
	ParentClass = ChosenParentClass ? ChosenParentClass : UGameRoot::StaticClass();

	if (!isSuccessful)
	{
		return false;
	}

	return true;
}

bool UGameRootFactory::ShouldShowInNewMenu() const
{
	return true;
}

UObject* UGameRootFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                            UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UGameRoot::StaticClass()));

	UClass* CurrentParentClass = ParentClass;
	if (CurrentParentClass == nullptr)
	{
		CurrentParentClass = UGameRoot::StaticClass();
	}

	return NewObject<UGameRoot>(InParent, CurrentParentClass, InName, Flags);
}

UObject* UGameRootFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                            UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(InClass, InParent, InName, Flags, Context, Warn, NAME_None);
}

// void FAssetTypeActions_GameRoot::OpenAssetEditor(const TArray<UObject*>& InObjects,
//                                                  TSharedPtr<IToolkitHost> EditWithinLevelEditor)
// {
// 	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
// }

const FSlateBrush* FAssetTypeActions_GameRoot::GetThumbnailBrush(const FAssetData& InAssetData,
                                                                 const FName InClassName) const
{
	const FSlateIcon Icon(TEXT("FoolishGameStyle"),TEXT("FoolishGameEditor.GameRoot"));
	return Icon.GetIcon();
}

const FSlateBrush* FAssetTypeActions_GameRoot::GetIconBrush(const FAssetData& InAssetData,
                                                            const FName InClassName) const
{
	return GetThumbnailBrush(InAssetData, InClassName);
}

#undef LOCTEXT_NAMESPACE
#endif