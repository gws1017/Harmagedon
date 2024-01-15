#include "Actor/Controller/BasicPlayerController.h"
#include "UI/HUDOverlay.h"
#include "UI/LevelUPUI.h"
#include "UI/PasueMenuUI.h"
#include "UI/RestartMenuUI.h"
#include "Global.h"

ABasicPlayerController::ABasicPlayerController()
{
}

void ABasicPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeUIInstance();
	
}

void ABasicPlayerController::InitializeUIInstance()
{
	if (!!HUDOverlayClass)
	{
		if (HUDOverlayInstance == nullptr)
			HUDOverlayInstance = CreateWidget<UHUDOverlay>(GetWorld(), HUDOverlayClass);
		CurrentWidget = HUDOverlayInstance;
		CurrentWidget->AddToViewport();
	}
	if (!!PasueMenuClass)
	{
		if (PasueMenuInstance == nullptr)
			PasueMenuInstance = CreateWidget<UPasueMenuUI>(GetWorld(), PasueMenuClass);
	}
	if (!!RestartMenuClass)
	{
		if (RestartMenuInstance == nullptr)
			RestartMenuInstance = CreateWidget<URestartMenuUI>(GetWorld(), RestartMenuClass);
	}

	if (!!LevelUPUIClass)
	{
		if (LevelUPUIInstance == nullptr)
			LevelUPUIInstance = CreateWidget<ULevelUPUI>(GetWorld(), LevelUPUIClass);
	}

}

void ABasicPlayerController::ToggleUI(UUserWidget* widget)
{
	if (widget->IsInViewport())
		RemoveGameUI(widget);
	else
		ShowGameUI(widget);
}

void ABasicPlayerController::ShowGameUI(UUserWidget* GameUI)
{
	if (GameUI)
	{
		bShowMouseCursor = true;
		bGameInputMode = true;
		SetInputMode(FInputModeGameAndUI{});
		if (!GameUI->IsInViewport())GameUI->AddToViewport();
		GameUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABasicPlayerController::RemoveGameUI(UUserWidget* GameUI)
{
	if (GameUI)
	{
		bShowMouseCursor = false;
		bGameInputMode = false;
		SetInputMode(FInputModeGameOnly{});
		if (GameUI->IsInViewport())GameUI->RemoveFromParent();
	}
}

void ABasicPlayerController::ChangeMenuWidget(UUserWidget* NewWidget)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (NewWidget != nullptr)
	{
		CurrentWidget = NewWidget;
		CurrentWidget->AddToViewport();
	}
}
