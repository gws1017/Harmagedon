#include "Actor/Controller/TitleController.h"
#include "UI/TitleUI.h"
#include "Global.h"

ATitleController::ATitleController()
{
}

void ATitleController::BeginPlay()
{
	Super::BeginPlay();

	if (!!TitleUIClass)
	{
		if (TitleUIInstance == nullptr)
			TitleUIInstance = Cast<UTitleUI>(CreateWidget(GetWorld(), TitleUIClass));
		TitleUIInstance->AddToViewport();
	}

	FInputModeUIOnly InputMode;

	SetInputMode(InputMode);
	bShowMouseCursor = true;
}
