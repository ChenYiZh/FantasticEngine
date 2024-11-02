// Fill out your copyright notice in the Description page of Project Settings.


#include "Basis/Table/TableFileReader.h"

#include "HttpModule.h"
#include "GameRoot.h"
#include "Common/StringConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "Utilities/PathUtility.h"

void FTableReadTask::DoWork()
{
	if (Table == nullptr)
	{
		Reader->Error = TEXT("传入的表结构为空！");
		Reader->bIsDone = true;
		return;
	}
	FString Content;
	bool bCanLoadOnline = Reader->bUseOnline;
	bool bUseLocal = true;
	if (bCanLoadOnline)
	{
		bUseLocal = Reader->LocalMd5 == Reader->OnlineMd5;
	}
	//本地表 完整名称
	FString FullPath = UPathUtility::TABLE_PATH() + Table->GetTableName();
	//FullPath.Append(bUseLocal ? ".txt" : ".zip");
	if constexpr (WITH_EDITOR)
	{
		FullPath.Append(TEXT(".txt"));
	}
	else
	{
		FullPath.Append(TEXT(".zip"));
	}
	bool IsZip = FullPath.EndsWith(TEXT(".zip"));
	if (bUseLocal)
	{
		//FullPath = FPaths::ConvertRelativePathToFull(FullPath);
		TArray<uint8> Buffer;
		bool bSuccess = FFileHelper::LoadFileToArray(Buffer, ToCStr(FullPath));
		//bool bSuccess = FFileHelper::LoadFileToString(Content, ToCStr(FullPath));
		if (!bSuccess)
		{
			// Reader->Error = FString::Printf(TEXT("%s 表文件读取失败！"), ToCStr(Table->GetTableName()));
			// Reader->bIsDone = true;
			// return;
			bUseLocal = false;
		}
		if (IsZip)
		{
			FString Md5 = FMD5::HashBytes(Buffer.GetData(), Buffer.Num());
			if (bUseLocal && !Md5.Equals(Reader->LocalMd5, ESearchCase::IgnoreCase))
			{
				bUseLocal = false;
			}
		}
		if (bUseLocal)
		{
			if (Buffer.IsEmpty())
			{
				bUseLocal = false;
			}
			if (bUseLocal)
			{
				Content = UStringConverter::FromUTF8(Buffer.GetData(), Buffer.Num());
				if (Content.IsEmpty())
				{
					// Reader->Error = FString::Printf(TEXT("%s 表文件为空表！"), ToCStr(Table->GetTableName()));
					// Reader->bIsDone = true;
					// return;
					bUseLocal = false;
				}
			}
		}
	}
	if (!bUseLocal)
	{
		if (bCanLoadOnline)
		{
			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
			HttpRequest->SetURL(GetGameDefines<UGameDefines>(Table)->GetTableUrl() + Table->GetTableName() + TEXT(".zip"));
			HttpRequest->SetVerb("GET");
			HttpRequest->ProcessRequest();
			while (HttpRequest->GetStatus() == EHttpRequestStatus::Processing)
			{
				FPlatformProcess::Sleep(1);
			}
			FHttpResponsePtr Response = HttpRequest->GetResponse();
			if (!Response.IsValid() || Response->GetResponseCode() != EHttpResponseCodes::Ok)
			{
				Reader->Error = TEXT("It is error from request the table: ") + Table->GetTableName();
			}
			else
			{
				const TArray<uint8> ResponseBuffer = Response->GetContent();
				FFileHelper::SaveArrayToFile(ResponseBuffer, ToCStr(FullPath));
				Content = Response->GetContentAsString();
			}
		}
		else
		{
			Reader->Error = FString::Printf(TEXT("%s fail to load！"), ToCStr(Table->GetTableName()));
		}
	}
	try
	{
		if (IsZip)
		{
			int32 Shift = GetGameDefines<UGameDefines>(Table)->GetTableShift();
			if (Shift > 0)
			{
				Content = UStringConverter::FastEncrypt(Content, -Shift);
			}
		}
		Table->Parse(Content);
	}
	catch (...)
	{
		Reader->Error = FString::Printf(TEXT("%s fail to convert content！"), ToCStr(Table->GetTableName()));
	}
	Reader->bIsDone = true;
}

TStatId FTableReadTask::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FTableReadTask, STATGROUP_ThreadPoolAsyncTasks);
}

void UTableFileReader::Read(UTableScheme* Table, const FString& InLocalMd5, const FString& InOnlineMd5,
                            const bool& bInUseOnline)
{
	bIsDone = false;
	LocalMd5 = InLocalMd5;
	OnlineMd5 = InOnlineMd5;
	bUseOnline = bInUseOnline;
	//Table->BeginRead();
	(new FAutoDeleteAsyncTask<FTableReadTask>(this, Table))->StartBackgroundTask();
}

bool UTableFileReader::IsDone() const
{
	return bIsDone;
}

bool UTableFileReader::IsError() const
{
	return !Error.IsEmpty();
}

const FString& UTableFileReader::GetError() const
{
	return Error;
}
