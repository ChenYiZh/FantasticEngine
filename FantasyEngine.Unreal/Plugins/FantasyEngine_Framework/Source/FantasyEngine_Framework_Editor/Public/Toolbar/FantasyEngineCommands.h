// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FantasyEngineStyle.h"
#include "Framework/Commands/Commands.h"


/** Framework的编辑器菜单按钮 */
class FFantasyEngineCommands : public TCommands<FFantasyEngineCommands>
{
public:
	FFantasyEngineCommands():
		TCommands<FFantasyEngineCommands>(TEXT("FantasyEngineFrameworkEditor"),
		                                NSLOCTEXT("Contexts", "Fantasy Engine Editor", "Fantasy Engine Editor"),
		                                NAME_None,
		                                FFantasyEngineStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	/** 主菜单消息 */
	TSharedPtr<FUICommandInfo> ToolbarAction;
	/** 创建表格 */
	TSharedPtr<FUICommandInfo> GenerateTables;
	/** 框架的配置信息 */
	TSharedPtr<FUICommandInfo> FrameworkSettings;
	/** 编辑器配置 */
	TSharedPtr<FUICommandInfo> FantasyEngineEditor;
	/** 世界创建的设置配置 */
	TSharedPtr<FUICommandInfo> WorldCreatorSettings;


	/** 模块地图生成面板 */
	TSharedPtr<FUICommandInfo> OpenPuzzleGeneratorWindow;
};
