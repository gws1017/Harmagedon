// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUI.generated.h"

class APlayerCharacter;

UCLASS()
class PROJECTH_API UBaseUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativePreConstruct();


protected:

	UPROPERTY(VisibleAnywhere)
		APlayerCharacter* OwnerCharacter;
};
