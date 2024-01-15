// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "DialogBoxUI.generated.h"

DECLARE_DELEGATE(FClickBtn);

UCLASS()
class PROJECTH_API UDialogBoxUI : public UBaseUI
{
	GENERATED_BODY()

public:

	virtual void NativeDestruct() override;

public:

	UFUNCTION(BlueprintCallable)
		void OnClickOKBtn();
	UFUNCTION(BlueprintCallable)
		void OnClickNoBtn();

public:

	FClickBtn ClickOKBtnFunction;
	FClickBtn ClickNoBtnFunction;
};
