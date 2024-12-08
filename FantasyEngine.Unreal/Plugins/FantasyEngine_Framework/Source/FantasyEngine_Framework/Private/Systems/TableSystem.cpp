// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/TableSystem.h"

#include "HttpModule.h"
#include "GameRoot.h"
#include "Basis/Tips/TipLoadingBasic.h"
#include "Defines/EventIdsBasic.h"
#include "Interfaces/IHttpResponse.h"
#include "Log/FEConsole.h"
#include "Systems/EventSystem.h"
#include "Utilities/ObjectFactory.h"
#include "Utilities/PathUtility.h"

void UTableSystem::InitTables(const TArray<UTableScheme*>& InTables)
{
	Tables = InTables;
	FPlatformAtomics::InterlockedExchange(&State, static_cast<int32>(ETableSystemState::Created));
}

void UTableSystem::ReadTables()
{
	constexpr int32 Created = static_cast<int32>(ETableSystemState::Created);
	constexpr int32 Ready = static_cast<int32>(ETableSystemState::Ready);
	constexpr int32 Begin = static_cast<int32>(ETableSystemState::Begin);
	if (FPlatformAtomics::InterlockedCompareExchange(&State, Begin, Created) == Created ||
		FPlatformAtomics::InterlockedCompareExchange(&State, Begin, Ready) == Ready
	)
	{
		(new FAutoDeleteAsyncTask<FTableSystemBeginLoadingTask>(this))->StartBackgroundTask();
	}
}

ETableSystemState UTableSystem::GetState() const
{
	return static_cast<ETableSystemState>(State);
}

void UTableSystem::OnTick_Implementation(float DeltaTime)
{
	Super::OnTick_Implementation(DeltaTime);
	constexpr int32 Loading = static_cast<int32>(ETableSystemState::Loading);
	if (FPlatformAtomics::AtomicRead(&State) == Loading)
	{
		UFEConsole::WriteWarnWithCategory(TEXT("Table System"), TEXT("TableSystem is Loading"), true, DeltaTime);
		for (int i = Readers.Num() - 1; i >= 0; i--)
		{
			UTableFileReader* Reader = Readers[i];
			if (Reader->IsDone())
			{
				if (Reader->IsError())
				{
					UFEConsole::WriteErrorWithCategory(TEXT("Table System"), Reader->GetError());
				}
				Readers.RemoveAt(i);
				Reader->ClearGarbage();
				Reader->MarkAsGarbage();
			}
		}
		if (Readers.IsEmpty())
		{
			FPlatformAtomics::InterlockedExchange(&State, static_cast<int32>(ETableSystemState::Ready));
			UEventSystem::SendEvent(this, UEventIdsBasic::GET_READY());
		}
	}
}

void UTableSystem::OnQuit_Implementation()
{
	for (UTableScheme* Table : Tables)
	{
		Table->ClearGarbage();
		Table->MarkAsGarbage();
	}
	Tables.Reset();
	Super::OnQuit_Implementation();
}

bool UTableSystem::IsReady_Implementation()
{
	constexpr int32 Ready = static_cast<int32>(ETableSystemState::Ready);
	return FPlatformAtomics::AtomicRead(&State) == Ready;
}

void FTableSystemBeginLoadingTask::DoWork()
{
	bool bIsEditor = WITH_EDITOR;
	bool bUseOnline = !bIsEditor;
	ETableLoadMode Mode = GetGameDefines<UGameDefines>(TableSystem)->GetTableLoadMode();
	if (Mode != ETableLoadMode::Default)
	{
		bUseOnline = Mode == ETableLoadMode::ForceOnline;
	}
	TMap<FString, FString> LocalMd5;
	TMap<FString, FString> OnlineMd5;
	FString Content;
	LoadMd5(LocalMd5, UPathUtility::TABLE_IDX_PATH(), true, Content);
	if (bUseOnline)
	{
		LoadMd5(OnlineMd5, GetGameDefines<UGameDefines>(TableSystem)->GetTableIdxUrl(), false, Content);
		if (!OnlineMd5.IsEmpty())
		{
			FFileHelper::SaveStringToFile(Content, ToCStr(UPathUtility::TABLE_IDX_PATH()),
			                              FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		}
	}
	if (OnlineMd5.IsEmpty())
	{
		for (auto KV : LocalMd5)
		{
			OnlineMd5.Add(KV);
		}
	}
	else
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		for (auto KV : LocalMd5)
		{
			if (!OnlineMd5.Contains(KV.Key))
			{
				//本地表 完整名称
				const TCHAR* FullPath = ToCStr(UPathUtility::TABLE_PATH() + KV.Key);
				if (PlatformFile.FileExists(FullPath))
				{
					PlatformFile.DeleteFile(FullPath);
				}
			}
		}
	}
	auto Tables = TableSystem->Tables;
	for (UTableScheme* Table : Tables)
	{
		Table->BeginRead();
		FString LocalTableMd5, OnlineTableMd5;
		if (LocalMd5.Contains(Table->GetTableName()))
		{
			LocalTableMd5 = LocalMd5[Table->GetTableName()];
		}
		if (OnlineMd5.Contains(Table->GetTableName()))
		{
			OnlineTableMd5 = OnlineMd5[Table->GetTableName()];
		}
		if ((bUseOnline && OnlineTableMd5.IsEmpty()) || (!bUseOnline && LocalTableMd5.IsEmpty()))
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("Table System"),
				FString::Printf(
					TEXT("The table: %s is not exists in configs assets."), ToCStr(Table->GetTableName())));
			continue;
		}
		UTableFileReader* Reader = NewObject<UTableFileReader>();
		TableSystem->Readers.Add(Reader);
		Reader->Read(Table, LocalTableMd5, OnlineTableMd5, bUseOnline);
	}
	ULoadingParam* Param = NewObject<ULoadingParam>(TableSystem);
	Param->Progress = 0;
	UEventSystem::SendEvent(TableSystem, UEventIdsBasic::LOADING_PROGRESS(), Param);
	FPlatformAtomics::InterlockedExchange(&TableSystem->State, static_cast<int32>(ETableSystemState::Loading));
}

TStatId FTableSystemBeginLoadingTask::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FTableSystemBeginLoadingTask, STATGROUP_ThreadPoolAsyncTasks);
}

void FTableSystemBeginLoadingTask::LoadMd5(TMap<FString, FString>& Md5Map, const FString& Uri, const bool& bIsLocal,
                                           FString& Content) const
{
	TArray<FString> Lines;
	if (bIsLocal)
	{
		FFileHelper::LoadFileToStringArray(Lines, ToCStr(Uri));
	}
	else
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetURL(Uri);
		HttpRequest->SetVerb("GET");
		HttpRequest->ProcessRequest();
		while (HttpRequest->GetStatus() == EHttpRequestStatus::Processing)
		{
			FPlatformProcess::Sleep(1);
		}
		FHttpResponsePtr Response = HttpRequest->GetResponse();
		if (Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
		{
			Content = Response->GetContentAsString();
			Content.ParseIntoArrayLines(Lines);
		}
		else
		{
			UFEConsole::WriteErrorWithCategory(TEXT("Table System"),TEXT("Fail to load online md5."));
		}
	}
	for (FString Line : Lines)
	{
		Line.RemoveFromEnd(TEXT("\r"));
		if (!Line.IsEmpty())
		{
			FString Name, Md5;
			Line.Split(TEXT("|"), &Name, &Md5);
			if (Name.IsEmpty() || Md5.IsEmpty())
			{
				continue;
			}
			Md5Map.Add(Name, Md5);
		}
	}
}
