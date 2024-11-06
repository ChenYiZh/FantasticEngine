// Fill out your copyright notice in the Description page of Project Settings.


#include "FEUtilities.h"

#if PLATFORM_WINDOWS
#include <Runtime\Core\Public\HAL\FileManager.h>
#include <Runtime\Core\Public\Misc\Paths.h>
#include <Runtime\Core\Public\Windows\COMPointer.h>
#endif

#if PLATFORM_MAC
// Access to Objective-C functions for C++.
#include <CoreFoundation/CoreFoundation.h>
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#include <objc/message.h>

// Access to MainThread wrapper.
#include "Mac/CocoaThread.h"

// Helpers for calling objective-c routines at runtime.
// The objc_msgSend function needs purpose-built casting.
#define id_OBJC_MSGSEND ((id (*)(id, SEL))objc_msgSend)
#define void_OBJC_MSGSEND_bool ((void (*)(id, SEL, bool))objc_msgSend)
#define id_OBJC_MSGSEND_id ((id (*)(id, SEL, id))objc_msgSend)
#define void_OBJC_MSGSEND_id ((void (*)(id, SEL, id))objc_msgSend)
#define id_OBJC_MSGSEND_cstr ((id (*)(id, SEL, const char *))objc_msgSend)
#define id_OBJC_MSGSEND_int ((id (*)(id, SEL, int))objc_msgSend)
#define cstr_OBJC_MSGSEND ((const char *(*)(id, SEL))objc_msgSend)
#define int_OBJC_MSGSEND ((int (*)(id, SEL))objc_msgSend)

#endif

#define MAX_FILETYPES_STR 4096
#define MAX_FILENAME_STR 65536

void UFEUtilities::OpenFileDialog(FString DialogTitle, const FString& FileTypes, TArray<FString>& Filenames,
                                  bool bMultiSelect)
{
	const void* ParentWindowHandle = nullptr;
	int OutFilterIndex;
	FileDialogShared(false, ParentWindowHandle, DialogTitle, FString(), FString(), FileTypes,
	                 bMultiSelect ? 0x01 : 0x00,
	                 Filenames, OutFilterIndex);
}

bool UFEUtilities::SaveFileDialog(FString DialogTitle, FString DefaultPath, FString DefaultFile,
                                  const FString& FileTypes, FString& Filename)
{
	const void* ParentWindowHandle = nullptr;
	int OutFilterIndex;
	TArray<FString> outFilenames;
	bool bSuccess = FileDialogShared(true, ParentWindowHandle, DialogTitle, DefaultPath, DefaultFile, FileTypes, 0x00,
	                                 outFilenames,
	                                 OutFilterIndex);
	if (bSuccess && outFilenames.Num() > 0)
	{
		Filename = outFilenames[0];
	}
	else
	{
		Filename = FString();
	}
	return bSuccess;
}

bool UFEUtilities::FileDialogShared(bool bSave, const void* ParentWindowHandle, const FString& DialogTitle,
                                    const FString& DefaultPath, const FString& DefaultFile,
                                    const FString& FileTypes, uint32 Flags,
                                    TArray<FString>& OutFilenames, int32& OutFilterIndex)
{
#pragma region Windows
	//FScopedSystemModalMode SystemModalScope;
#if PLATFORM_WINDOWS
	WCHAR Filename[MAX_FILENAME_STR];
	FCString::Strcpy(Filename, MAX_FILENAME_STR, *(DefaultFile.Replace(TEXT("/"), TEXT("\\"))));

	// Convert the forward slashes in the path name to backslashes, otherwise it'll be ignored as invalid and use whatever is cached in the registry
	WCHAR Pathname[MAX_FILENAME_STR];
	FCString::Strcpy(Pathname, MAX_FILENAME_STR,
	                 *(FPaths::ConvertRelativePathToFull(DefaultPath).Replace(TEXT("/"), TEXT("\\"))));

	// Convert the "|" delimited list of filetypes to NULL delimited then add a second NULL character to indicate the end of the list
	//WCHAR FileTypeStr[MAX_FILETYPES_STR];
	//const int32 FileTypesLen = FileTypes.Len();
	//WCHAR FileTypesPtr[MAX_FILENAME_STR];
	WCHAR FileTypesPtr[MAX_FILENAME_STR];
	for (int32 i = 0; i < MAX_FILENAME_STR; i++)
	{
		FileTypesPtr[i] = '\0';
	}
	int32 Size = FileTypes.Len();
	FCString::Strcpy(FileTypesPtr, Size, *FileTypes);
	for (int32 i = 0; i < Size; i++)
	{
		if (FileTypesPtr[i] == '|')
		{
			FileTypesPtr[i] = '\0';
		}
	}
	// Nicely formatted file types for lookup later and suitable to append to filenames without extensions
	TArray<FString> CleanExtensionList;

	// The strings must be in pairs for windows.
	// It is formatted as follows: Pair1String1|Pair1String2|Pair2String1|Pair2String2
	// where the second string in the pair is the extension.  To get the clean extensions we only care about the second string in the pair
	// TArray<FString> UnformattedExtensions;
	// FileTypes.ParseIntoArray(UnformattedExtensions, TEXT("|"), true);
	// for (int32 ExtensionIndex = 1; ExtensionIndex < UnformattedExtensions.Num(); ExtensionIndex += 2)
	// {
	// 	const FString& Extension = UnformattedExtensions[ExtensionIndex];
	// 	// Assume the user typed in an extension or doesnt want one when using the *.* extension. We can't determine what extension they wan't in that case
	// 	if (Extension != TEXT("*.*"))
	// 	{
	// 		// Add to the clean extension list, first removing the * wildcard from the extension
	// 		const int32 WildCardIndex = Extension.Find(TEXT("*"));
	// 		CleanExtensionList.Add(WildCardIndex != INDEX_NONE ? Extension.RightChop(WildCardIndex + 1) : Extension);
	// 	}
	// }


	// if (FileTypesLen > 0 && FileTypesLen - 1 < MAX_FILETYPES_STR)
	// {
	// 	//FileTypesPtr = *FileTypes;
	// 	FCString::Strncpy(FileTypesPtr, *FileTypes, FileTypesLen);
	// 	// FCString::Strcpy(FileTypeStr, MAX_FILETYPES_STR, *FileTypes);
	// 	// TCHAR* Pos = FileTypeStr;
	// 	// while (Pos[0] != 0)
	// 	// {
	// 	// 	if (Pos[0] == '|')
	// 	// 	{
	// 	// 		Pos[0] = 0;
	// 	// 	}
	// 	//
	// 	// 	Pos++;
	// 	// }
	// 	//
	// 	// // Add two trailing NULL characters to indicate the end of the list
	// 	// FileTypeStr[FileTypesLen] = 0;
	// 	// FileTypeStr[FileTypesLen + 1] = 0;
	// }

	OPENFILENAME ofn;
	FMemory::Memzero(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = (HWND)ParentWindowHandle;
	ofn.lpstrFilter = FileTypesPtr;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;
	ofn.nMaxFile = MAX_FILENAME_STR;
	ofn.lpstrInitialDir = Pathname;
	ofn.lpstrTitle = *DialogTitle;

	//ofn.lpstrDefExt = *FileTypes[0];
	// if (FileTypesLen > 0)
	// {
	// 	ofn.lpstrDefExt = *UnformattedExtensions[0]; //&FileTypeStr[0];
	// }

	ofn.Flags = OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER;

	if (bSave)
	{
		ofn.Flags |= OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOVALIDATE;
	}
	else
	{
		ofn.Flags |= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	}

	if (Flags & 0x01)
	{
		ofn.Flags |= OFN_ALLOWMULTISELECT;
	}

	bool bSuccess;
	if (bSave)
	{
		bSuccess = !!::GetSaveFileName(&ofn);
	}
	else
	{
		bSuccess = !!::GetOpenFileName(&ofn);
	}

	if (bSuccess)
	{
		// GetOpenFileName/GetSaveFileName changes the CWD on success. Change it back immediately.
		//FPlatformProcess::SetCurrentWorkingDirectoryToBaseDir();

		if (Flags & 0x01)
		{
			// When selecting multiple files, the returned string is a NULL delimited list
			// where the first element is the directory and all remaining elements are filenames.
			// There is an extra NULL character to indicate the end of the list.
			FString DirectoryOrSingleFileName = FString(Filename);
			TCHAR* Pos = Filename + DirectoryOrSingleFileName.Len() + 1;

			if (Pos[0] == 0)
			{
				// One item selected. There was an extra trailing NULL character.
				OutFilenames.Add(DirectoryOrSingleFileName);
			}
			else
			{
				// Multiple items selected. Keep adding filenames until two NULL characters.
				FString SelectedFile;
				do
				{
					SelectedFile = FString(Pos);
					new(OutFilenames) FString(DirectoryOrSingleFileName / SelectedFile);
					Pos += SelectedFile.Len() + 1;
				}
				while (Pos[0] != 0);
			}
		}
		else
		{
			new(OutFilenames) FString(Filename);
		}

		// The index of the filter in OPENFILENAME starts at 1.
		OutFilterIndex = ofn.nFilterIndex - 1;

		// Get the extension to add to the filename (if one doesnt already exist)
		FString Extension = CleanExtensionList.IsValidIndex(OutFilterIndex)
			                    ? CleanExtensionList[OutFilterIndex]
			                    : TEXT("");

		// Make sure all filenames gathered have their paths normalized and proper extensions added
		for (auto OutFilenameIt = OutFilenames.CreateIterator(); OutFilenameIt; ++OutFilenameIt)
		{
			FString& OutFilename = *OutFilenameIt;

			OutFilename = IFileManager::Get().ConvertToRelativePath(*OutFilename);

			if (FPaths::GetExtension(OutFilename).IsEmpty() && !Extension.IsEmpty())
			{
				// filename does not have an extension. Add an extension based on the filter that the user chose in the dialog
				OutFilename += Extension;
			}

			FPaths::NormalizeFilename(OutFilename);
		}
	}
	else
	{
		uint32 Error = ::CommDlgExtendedError();
		if (Error != ERROR_SUCCESS)
		{
			//UE_LOG(LogDesktopPlatform, Warning, TEXT("Error reading results of file dialog. Error: 0x%04X"), Error);
		}
	}

	return bSuccess;
#endif
#pragma endregion

#pragma region LINUX
#if PLATFORM_LINUX
	return false;
#endif
#pragma endregion

#pragma region MAC
#if PLATFORM_MAC

    bool bSuccess = false;

    OutFilenames.Empty();

    // TODO: honor multi select flag.
    // TODO: set dialog title
    // TODO: set default file
    // TODO: enable file type filters.

    MainThreadCall(^{
        SCOPED_AUTORELEASE_POOL;
        id panel = id_OBJC_MSGSEND((id)objc_getClass("NSOpenPanel"), sel_getUid("openPanel"));
        void_OBJC_MSGSEND_bool(panel, sel_getUid("setCanChooseFiles:"), YES);
        void_OBJC_MSGSEND_bool(panel, sel_getUid("setCanChooseDirectories:"), NO);
        void_OBJC_MSGSEND_bool(panel, sel_getUid("setAllowsMultipleSelection:"), YES);
        void_OBJC_MSGSEND_id(panel, sel_getUid("setDirectoryURL:"),
            id_OBJC_MSGSEND_id((id)objc_getClass("NSURL"), sel_getUid("fileURLWithPath:"),
            id_OBJC_MSGSEND_cstr((id)objc_getClass("NSString"), sel_getUid("stringWithUTF8String:"), TCHAR_TO_UTF8(&DefaultPath))));
        int response = int_OBJC_MSGSEND(panel, sel_getUid("runModal"));

        if (response == NSModalResponseOK) {
            id URLs = id_OBJC_MSGSEND(panel, sel_getUid("URLs"));
            int num_URLs = int_OBJC_MSGSEND(URLs, sel_getUid("count"));
            for (int i = 0; i < num_URLs; ++i) {
                id fileURL = id_OBJC_MSGSEND_int(URLs, sel_getUid("objectAtIndex:"), i);
                id path = id_OBJC_MSGSEND(fileURL, sel_getUid("path"));
                const char *path_utf8 = cstr_OBJC_MSGSEND(path, sel_getUid("UTF8String"));
	        OutFilenames.Add(FString(path_utf8));
            }
        }
    }, UnrealShowEventMode, true);

    return OutFilenames.Num() > 0;

#endif
#pragma endregion
}
