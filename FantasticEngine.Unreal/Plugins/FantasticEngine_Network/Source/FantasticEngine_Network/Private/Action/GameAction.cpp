/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
https://space.bilibili.com/9308172

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/
#include "Action/GameAction.h"

#include "Log/FEConsole.h"
#include "Log/FENCategories.h"

long UGameAction::GetMsgId() const
{
	return mMsgId;
}

int UGameAction::GetActionId() const
{
	return mActionId;
}

bool UGameAction::GetAnalysisTime() const
{
	return bAnalysisTime;
}

int UGameAction::GetAlertTimeout() const
{
	return AlertTimeout;
}

FName UGameAction::GetTypeName() const
{
	return GetClass()->GetFName();
}

void UGameAction::Work(int InActionId, UMessageReader* InReader)
{
	mMsgId = InReader->GetMsgId();
	mActionId = InActionId;
	mReader = InReader;
	bAnalysisTime = true;
	AlertTimeout = 1000;
	try
	{
		Awake();
		EnterTime = FDateTime::Now();
		SetReader(InReader);
		if (Check())
		{
			TakeAction(InReader);
		}
		if (GetAnalysisTime())
		{
			FTimespan DeltaTime = FDateTime::Now() - EnterTime;
			if (DeltaTime.GetTotalMilliseconds() > AlertTimeout)
			{
				UFEConsole::WriteWarnWithCategory(UFENCategories::ACTION, FString::Printf(
					                                 TEXT("%s is timeout, %dms."), ToCStr(GetTypeName().ToString()),
					                                 static_cast<int>(DeltaTime.GetTotalMilliseconds())));
			}
		}
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::ACTION, FString::Printf(
			                                  TEXT("An error occurred on process action %s"),
			                                  ToCStr(GetTypeName().ToString())));
	}
	if (InReader->IsRooted())
	{
		InReader->RemoveFromRoot();
	}
	InReader->ClearGarbage();
	InReader->MarkAsGarbage();
	ClearGarbage();
	MarkAsGarbage();
}

void UGameAction::Awake_Implementation()
{
}

bool UGameAction::Check_Implementation()
{
	return true;
}

void UGameAction::SetReader_Implementation(UMessageReader* Reader)
{
}

void UGameAction::TakeAction_Implementation(UMessageReader* Reader)
{
}
