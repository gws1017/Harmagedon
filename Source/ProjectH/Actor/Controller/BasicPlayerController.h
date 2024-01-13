// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPlayerController.generated.h"

class UHUDOverlay;
UCLASS()
class PROJECTH_API ABasicPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasicPlayerController();

public:
	virtual void BeginPlay() override;

public:

	void ChangeMenuWidget(UUserWidget* NewWidget);

private:

	void InitializeUIInstance();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UHUDOverlay> HUDOverlayClass;
	UPROPERTY()
		UHUDOverlay* HUDOverlayInstance;
	UPROPERTY(VisibleAnywhere)
		UUserWidget* CurrentWidget;
};
