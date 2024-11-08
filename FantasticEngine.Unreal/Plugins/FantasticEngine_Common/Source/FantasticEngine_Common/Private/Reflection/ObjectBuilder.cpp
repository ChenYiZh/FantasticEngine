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


#include "Reflection/ObjectBuilder.h"

#include "Engine/ObjectLibrary.h"

bool UObjectBuilder::IsBlueprintName(const FString& ClassName)
{
	return ClassName.Contains("'");
}

UObject* UObjectBuilder::CreateObjectByName(UObject* WorldContextObject, const FString& ClassName,
                                            TSubclassOf<UObject> SubClass)
{
	FScopeLock SetLock(&Mutex);
	if (!Types.Contains(ClassName))
	{
		UClass* Class;
		//蓝图类
		if (IsBlueprintName(ClassName))
		{
			// AsyncTask(ENamedThreads::GameThread, []()
			// {
			// });
			UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, ToCStr(ClassName));
			if (Blueprint == nullptr)
			{
				Class = nullptr;
			}
			else
			{
				Class = Blueprint->GeneratedClass;
			}
		}
		else //C++代码
		{
			Class = FindObject<UClass>(nullptr, ToCStr(ClassName));
		}
		if (Class == nullptr)
		{
			return nullptr;
		}
		FStaticConstructObjectParameters Params(Class);
		Types.Add(ClassName, Params);
	}
	FStaticConstructObjectParameters Params = Types[ClassName];
	Params.Outer = WorldContextObject;
	return StaticConstructObject_Internal(Params);
}

// void UObjectBuilder::FindAllBlueprintClassFromPath(TArray<UClass*>& Result, const FString& Path,
//                                                    const FString& LikeName)
// {
// 	bool bCheckName = !LikeName.IsEmpty();
// 	UObjectLibrary* MyObjectLibrary = UObjectLibrary::CreateLibrary(AMyActor::StaticClass(), true, true);
//
// 	MyObjectLibrary ->LoadBlueprintAssetDataFromPath(TEXT("/Game/PATH"));
// 	MyObjectLibrary ->LoadAssetsFromAssetData();
//
// 	TArray assetDataList;
// 	MyObjectLibrary ->GetAssetDataList(assetDataList);
//
// 	for (FAssetData& assetData : assetDataList)
// 	{
// 		if (UBlueprint* blueprintObject = Cast(assetData.GetAsset()))
// 		{
// 			if (UBlueprintGeneratedClass* blueprintGeneratedClass = Cast(blueprintObject->GeneratedClass.Get()))
// 			{
// 				const AMyActor* actorDC = Cast(blueprintGeneratedClass->ClassDefaultObject);
// 				UClass* objClass = AMyActor->GetClass();
// 			}
// 		}
// 	}
// }
