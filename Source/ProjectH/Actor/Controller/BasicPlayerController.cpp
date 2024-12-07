#include "Actor/Controller/BasicPlayerController.h"
#include "UI/HUDOverlay.h"
#include "UI/LevelUPUI.h"
#include "UI/PasueMenuUI.h"
#include "UI/RestartMenuUI.h"
#include "UI/EquipmentUI.h"
#include "UI/InventoryUI.h"
#include "UI/OverlapUI.h"
#include "UI/BossHUDWidget.h"
#include "UI/SettingMenuUI.h"
#include "Global.h"


ABasicPlayerController::ABasicPlayerController()
{
}

void ABasicPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeUIInstance();
	SetInputMode(FInputModeGameOnly{});

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

	if (!!EquipmentUIClass)
	{
		if (EquipmentUIInstance == nullptr)
			EquipmentUIInstance = CreateWidget<UUserWidget>(GetWorld(), EquipmentUIClass);
	}

	if (!!InventoryUIClass)
	{
		if (InventoryUIInstance == nullptr)
			InventoryUIInstance = CreateWidget<UInventoryUI>(GetWorld(), InventoryUIClass);
	}

	if (!!SettingUIClass)
	{
		if (SettingUIInstance == nullptr)
			SettingUIInstance = CreateWidget<USettingMenuUI>(GetWorld(), SettingUIClass);
	}

	if (!!OverlapUIClass)
	{
		if (OverlapUIInstance == nullptr)
			OverlapUIInstance = CreateWidget<UOverlapUI>(GetWorld(), OverlapUIClass);
	}

	//if (!!MenuUIClass)
	//{
	//	if (MenuUIInstance == nullptr)
	//		MenuUIInstance = CreateWidget<UInventoryUI>(GetWorld(), MenuUIClass);
	//}

	// 위젯 생성하고 화면에 띄움
	if (!!BossHUDWidgetClass)
	{
		if (BossHUDWidget == nullptr)
			BossHUDWidget = CreateWidget<UBossHUDWidget>(GetWorld(), BossHUDWidgetClass);
		if (BossHUDWidget)
		{
			BossHUDWidget->AddToViewport();
			BossHUDWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void ABasicPlayerController::ToggleUI(UUserWidget* widget, bool ShowCursor)
{
	CheckNull(widget);
	if (widget->IsInViewport())
		RemoveGameUI(widget);
	else
		ShowGameUI(widget, ShowCursor);
}

void ABasicPlayerController::ShowGameUI(UUserWidget* GameUI, bool ShowCursor)
{
	if (GameUI)
	{
		bShowMouseCursor = ShowCursor;
		bGameInputMode = ShowCursor;
		if(ShowCursor)
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

void ABasicPlayerController::ShowRestartMenu()
{
	ShowGameUI(RestartMenuInstance);
}

void ABasicPlayerController::ShowInventoryMenu()
{
	ShowGameUI(InventoryUIInstance);
}

void ABasicPlayerController::ToggleOverlapUI(FText OvelapText)
{
	OverlapUIInstance->OverlapText = OvelapText;
	ToggleUI(OverlapUIInstance,false);
}

void ABasicPlayerController::ToggleEquipMenu()
{
	ToggleUI(EquipmentUIInstance);
}

void ABasicPlayerController::ToggleInventoryMenu()
{
	ToggleUI(InventoryUIInstance);
}

void ABasicPlayerController::ShowLoadingScreen()
{
	CheckNull(HUDOverlayInstance);
	HUDOverlayInstance->ShowLoadingTexture();
}

void ABasicPlayerController::HideLoadingScreen()
{
	CheckNull(HUDOverlayInstance);
	HUDOverlayInstance->HideLoadingTexture();
}

 /*************************************************************************************************
 * 보스방 입장 트리거에 플레이어가 들어왔을 때 컨트롤러를 가져와 이 함수를 호출
 * 보스 HUD를 화면에 표시
 * 
 * @author	조현식
 * @date	2024-05-26
 * @param	액터
 * @return	
 **************************************************************************************************/
void ABasicPlayerController::ShowBossHUD(AActor* EnemyPawn)
{
	BossHUDWidget->InitBar(EnemyPawn);
	BossHUDWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABasicPlayerController::HideBossHUD()
{
	BossHUDWidget->SetVisibility(ESlateVisibility::Hidden);
}

class UBossHUDWidget* ABasicPlayerController::GetBossHUD()
{
	return BossHUDWidget;
}
