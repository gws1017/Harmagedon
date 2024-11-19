// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "OverlapUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UOverlapUI : public UBaseUI
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
	FText OverlapText;
};
