// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDefines.h"
#include "Basis/SystemBasis.h"
#include "Containers/Ticker.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "Systems/EventSystem.h"
#include "GameRoot.generated.h"

UENUM(BlueprintType)
enum class EGameRootState:uint8
{
	Invalid = 0,
	Starting = 1,
	Ready = 2,
	Initialized = 3,
	Shutdown = 4,
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FANTASYENGINE_FRAMEWORK_API UGameRoot : public UWorldObject
{
	GENERATED_BODY()
	friend class UFEGameInstance;

private:
	/**
	 * 改动设置Root对象改为通过GameInstance获取，测试时多实例可能报错
	 * ChenYiZh 2024/4/25
	 */
	/** 单例 */
	//UPROPERTY()
	//inline static UGameRoot* Root = nullptr;
	// /** 挂载的全局对象 */
	// UPROPERTY()
	// UDefaultGameInstance* GameInstance;

public:
	/** 获取跟世界，如果报黄可换 GameRootUtility 相应的接口 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantasy Engine|Game Root",
		meta=(WorldContext="WorldContextObject", DeterminesOutputType="Class"))
	static UWorld* GetRootWorld(const UObject* WorldContextObject);

public:
	/** 获取全局单例，如果报黄可换 GameRootUtility 相应的接口 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantasy Engine|Game Root",
		meta=(WorldContext="WorldContextObject", DeterminesOutputType="Class"))
	static UFEGameInstance* GetGameInstance(const UObject* WorldContextObject,
	                                             TSubclassOf<UFEGameInstance> Class = nullptr);

public:
	/** 获取单例，如果报黄可换 GameRootUtility 相应的接口 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantasy Engine|Game Root",
		meta=(WorldContext="WorldContextObject", DeterminesOutputType="Class"))
	static UGameRoot* GetRoot(const UObject* WorldContextObject, TSubclassOf<UGameRoot> Class = nullptr);

	// protected:
	// 	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	// 	bool Enabled = true;

public:
	/** 需要有个地方存放单例，不然会被UE自动回收 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Fantasy Engine|Game Root")
	TMap<UClass*, UObject*> SingletonPool;

private:
	/** 当前状态 */
	EGameRootState State = EGameRootState::Invalid;

	/** Level脚本用户判断场景有没有加载完成 */
	UPROPERTY()
	ALevelScriptActor* FirstLevelActor;

	/** 帧事件句柄 */
	FTSTicker::FDelegateHandle TickHandler;
	/** 每秒任务计时器 */
	float Seconds;

public:
	/** 实例状态，如果报黄可换 GameRootUtility 相应的接口 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantasy Engine|Game Root", meta=(WorldContext="WorldContextObject"))
	static EGameRootState GetState(const UObject* WorldContextObject);

private:
	/** 配置信息 */
	UPROPERTY(VisibleInstanceOnly, Category="Fantasy Engine|Game Root|Configs")
	UGameDefines* GameDefines;


	/** 配置信息 */
	UPROPERTY(EditDefaultsOnly, Category="Fantasy Engine|Game Root|Configs")
	TSubclassOf<UGameDefines> GameDefinesClass = UGameDefines::StaticClass();

public:
	/** 全局配置信息，如果报黄可换 GameRootUtility 相应的接口 */
	UFUNCTION(Exec, BlueprintPure, Category="Fantasy Engine|Game Root",
		meta=(WorldContext="WorldContextObject", DeterminesOutputType=Class))
	static UGameDefines* GetGameDefines(const UObject* WorldContextObject, TSubclassOf<UGameDefines> Class = nullptr);

	/** 判断运行模式是否是Debug，如果报黄可换 GameRootUtility 相应的接口 */
	UFUNCTION(BlueprintPure, Exec, Category="Fantasy Engine|Game Root", meta=(WorldContext="WorldContextObject"))
	static bool IsDebug(const UObject* WorldContextObject);

protected:
	/** 全局系统 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fantasy Engine|Game Root")
	TSet<USystemBasis*> Systems;

public:
	/** 初始化操作 */
	virtual void Init(); // override;
	/** 帧事件 */
	virtual bool Tick(float DeltaTime);
	/** 结束事件 */
	virtual void Shutdown(); // override;

public:
	/** 初始化判断 */
	UFUNCTION(BlueprintPure)
	bool Initialized() const;

public:
	/** 监听事件注册 */
	virtual void PostInitProperties() override;
	/** 监听事件注销 */
	virtual void BeginDestroy() override;

private:
	/** 切换到后台 */
	virtual void OnApplicationWillEnterBackgroundDelegate_Handler();
	/** 切换到前台 */
	virtual void OnApplicationHasEnteredForegroundDelegate_Handler();
	/** 性能过低 */
	virtual void OnApplicationShouldUnloadResourcesDelegate_Handler();

protected:
	/** 准备事件 */
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Internal")
	//void GetReady(UEventParam* Param = nullptr);
	bool Ready();
	/** 启动事件 */
	void StartUp(UEventParam* Param = nullptr);

private:
	void OnInitialize();
	
protected:
	/** 窗体或者全局过滤器注册函数 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Internal")
	void Registing();
	/** 初始化函数覆写位置 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Internal")
	void OnInitialized();
	/** Event事件注册函数 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Internal")
	void RegistEvents();
	/** Event事件注销函数 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Internal")
	void UnregistEvents();
	/** 低内存事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Internal")
	void OnLowMemory();

protected:
	/** 创建时调用大部分参数还未赋值 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnCreate();
	/** 进入准备阶段事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnReady();
	/** 登录事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnLogin();
	/** 退出事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnLogout();
	/** 重载事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnReset();
	/** 每帧事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnTick(float DeltaTime);
	/** 每秒事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnEverySecond();
	/** 应用进入前台事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnApplicationFocus(bool Focus);
	/** 应用暂停事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnApplicationPause(bool Pause);
	/** 退出事件 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Events")
	void OnQuit();

public:
	/** 执行GameDefines下配置的 ConsoleCommands */
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"), Category="Fantasy Engine|Game Root|Cmd")
	static void ExecuteConsoleCommands(const UObject* WorldContextObject);

public:
	/**
	 * ChenYiZh 2023/5/21: 控制台指令，在Lua下使用
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantasy Engine|Game Root|Cmd")
	void Execute(const TArray<FString>& Cmds);
};

/** 获取配置文件 */
template <class T>
FORCEINLINE T* GetGameDefines(const UObject* WorldContextObject)
{
	return Cast<T>(UGameRoot::GetGameDefines(WorldContextObject));
}
