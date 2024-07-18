// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "TitleUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UTitleUI : public UBaseUI
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

public:

	UFUNCTION(BlueprintCallable)
		void OnClickNewGameButton();

	UFUNCTION(BlueprintCallable)
		void OnClickContinueButton();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanContinue = false;
};
