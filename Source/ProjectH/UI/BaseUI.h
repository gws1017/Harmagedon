// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUI.generated.h"

class APlayerCharacter;
class ABasicPlayerController;

UCLASS()
class PROJECTH_API UBaseUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativePreConstruct();


public:
	UFUNCTION(BlueprintPure)
	APlayerCharacter* GetOwnerCharacater();
	UFUNCTION(BlueprintCallable)
		ABasicPlayerController* GetOwnerController();

protected:

	UPROPERTY(VisibleAnywhere)
		APlayerCharacter* OwnerCharacter;
	UPROPERTY(VisibleAnywhere)
		ABasicPlayerController* OwnerController;
};
