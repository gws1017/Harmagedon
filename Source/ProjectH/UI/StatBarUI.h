// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "StatBarUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UStatBarUI : public UBaseUI
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		float GetHPPercent();

	UFUNCTION(BlueprintPure)
		float GetStaminaPercent();
};
