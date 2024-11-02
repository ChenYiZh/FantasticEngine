// Fill out your copyright notice in the Description page of Project Settings.
/*********************************************
ChenYiZh 源代码:
玩家操作类

修改 ChenYiZh 2023/5/9：
	添加2D琐视角操作
	隐含需求，相机初始角度是否需要作为默认配置

修改 ChenYiZh 2023/5/21：
	添加2.5d视角
	调整原本2d结构
*********************************************/
#pragma once

#include "CoreMinimal.h"
#include "BasePlayerControllerComponent.h"
#include "GameRoot.h"
#include "Components/Pawn/BasicOperationsComponent.h"
#include "Components/Pawn/PawnOperationsComponent.h"
#include "PrimitivePlayerControllerComponent.generated.h"


/** Framework Player Controller Component*/
UCLASS(Blueprintable, BlueprintType, ClassGroup="Fantasy Engine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UPrimitivePlayerControllerComponent : public UBasePlayerControllerComponent
{
	GENERATED_BODY()
#pragma region 操作管理

public:
	/** 前后操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void MoveForward(const FInputActionInstance& Instance);
	/** 左右操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void MoveRight(const FInputActionInstance& Instance);
	/** 视角上下移动操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void LookUp(const FInputActionInstance& Instance);
	/** 视角左右旋转操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void Turn(const FInputActionInstance& Instance);
	/** 视角前后旋转操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void ViewClose(const FInputActionInstance& Instance);
	/** 鼠标显示控制 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Mouse|Actions")
	void MouseControlPressed(const FInputActionInstance& Instance);
	/** 鼠标显示控制 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Mouse|Actions")
	void MouseControlReleased(const FInputActionInstance& Instance);
#pragma endregion

protected:
	/** 绑定按键事件 */
	virtual void BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines) override;

	UFUNCTION(BlueprintPure, Category="Controller")
	void GetBasicOperationsComponents(TArray<UBasicOperationsComponent*>& OperationsComponents) const;

	UFUNCTION(BlueprintPure, Category="Controller")
	void GetPawnOperationsComponents(TArray<UPawnOperationsComponent*>& OperationsComponents) const;

#pragma region Variables

public:
	/** 是否静止鼠标输入 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Controller|Variables")
	bool bLockTurnAndLookUp = false;
#pragma endregion
};
