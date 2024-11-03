/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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


#include "Proxy/ClientActionDispatcher.h"

#include "Log/FEConsole.h"
#include "Log/FENCategories.h"
#include "Proxy/ActionBoss.h"
#include "Reflection/ObjectBuilder.h"

void UClientActionDispatcher::Provide(UMessageReader* Message)
{
	FScopeLock SetLock(&Mutex);
	Messages.Push(Message);
}

bool UClientActionDispatcher::Tick(float DeltaSeconds)
{
	FScopeLock Lock(&Mutex);
	//转到新队列，防止占用时间太长
	TArray<UMessageReader*> Readers(Messages);
	Messages.Empty();
	Lock.Unlock();

	for (UMessageReader* Message : Readers)
	{
		for (const FString& ActionNameFormat : ActionNameFormats)
		{
			const TCHAR* Format = ToCStr(ActionNameFormat);
			FStringFormatOrderedArguments Args;
			Args.Add(Message->GetActionId());
			FString TypeName = FString::Format(Format, Args);
			UClientAction* Action = UObjectBuilder::Create<UClientAction>(this, TypeName);
			if (Action != nullptr)
			{
				try
				{
					UActionBoss::Exploit(Action, Message->GetActionId(), Message);
				}
				catch (...)
				{
					UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET, TEXT("Action error."));
				}
				break;
			}
		}
		if (Message != nullptr)
		{
			if (Message->IsRooted())
			{
				Message->RemoveFromRoot();
			}
			Message->ClearGarbage();
			Message->MarkAsGarbage();
		}
	}
	return true;
}

void UClientActionDispatcher::PostInitProperties()
{
	Super::PostInitProperties();
	TimeHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UClientActionDispatcher::Tick));
}

void UClientActionDispatcher::BeginDestroy()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TimeHandle);
	Super::BeginDestroy();
}
