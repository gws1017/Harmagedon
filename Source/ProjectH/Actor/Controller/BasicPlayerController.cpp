#include "Actor/Controller/BasicPlayerController.h"
#include "UI/HUDOverlay.h"
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
