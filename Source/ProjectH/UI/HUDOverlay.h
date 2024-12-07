// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "HUDOverlay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UHUDOverlay : public UBaseUI
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void ShowLoadingTexture();
	UFUNCTION(BlueprintImplementableEvent)
	void HideLoadingTexture();

};
