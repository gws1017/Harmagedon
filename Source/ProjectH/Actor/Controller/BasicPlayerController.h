// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPlayerController.generated.h"

class UHUDOverlay;
class URestartMenuUI;
class UPasueMenuUI;
class ULevelUPUI;

UCLASS()
class PROJECTH_API ABasicPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasicPlayerController();

public:
	virtual void BeginPlay() override;

public:

	FORCEINLINE bool GetGameInputMode() const { return bGameInputMode; }

	void ChangeMenuWidget(UUserWidget* NewWidget);

private:

	void InitializeUIInstance();

	UFUNCTION(BlueprintCallable)
		void ToggleUI(UUserWidget* widget);
	UFUNCTION()
		void ShowGameUI(UUserWidget* GameUI);
	UFUNCTION()
		void RemoveGameUI(UUserWidget* GameUI);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UHUDOverlay> HUDOverlayClass;
	UPROPERTY()
		UHUDOverlay* HUDOverlayInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UPasueMenuUI> PasueMenuClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UPasueMenuUI* PasueMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<URestartMenuUI> RestartMenuClass;
	UPROPERTY(BlueprintReadOnly)
		URestartMenuUI* RestartMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<ULevelUPUI> LevelUPUIClass;
	UPROPERTY(BlueprintReadOnly)
		ULevelUPUI* LevelUPUIInstance;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* CurrentWidget;

	bool bGameInputMode;
};
