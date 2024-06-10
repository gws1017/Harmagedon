#include "Actor/Controller/BasicPlayerController.h"
#include "UI/HUDOverlay.h"
#include "UI/LevelUPUI.h"
#include "UI/PasueMenuUI.h"
#include "UI/RestartMenuUI.h"
#include "UI/EquipmentUI.h"
#include "UI/InventoryUI.h"
#include "Global.h"

#include "UI/BossHUDWidget.h"

ABasicPlayerController::ABasicPlayerController()
{
	// HUD �������Ʈ ��������
	static ConstructorHelpers::FClassFinder<UBossHUDWidget> PdHUDWidgetRef(TEXT("/Game/UI/Blueprint/WBP_BossHUD.WBP_BossHUD_C"));
	if (PdHUDWidgetRef.Class)
	{
		BossHUDWidgetClass = PdHUDWidgetRef.Class;
	}
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

	// ���� �����ϰ� ȭ�鿡 ���
	BossHUDWidget = CreateWidget<UBossHUDWidget>(GetWorld(), BossHUDWidgetClass);
	if (BossHUDWidget)
	{
		BossHUDWidget->AddToViewport();
		BossHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABasicPlayerController::ToggleUI(UUserWidget* widget)
{
	CheckNull(widget);
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

void ABasicPlayerController::ShowRestartMenu()
{
	ShowGameUI(RestartMenuInstance);
}

void ABasicPlayerController::ShowInventoryMenu()
{
	ShowGameUI(InventoryUIInstance);
}

void ABasicPlayerController::ToggleEquipMenu()
{
	ToggleUI(EquipmentUIInstance);
}

void ABasicPlayerController::ToggleInventoryMenu()
{
	ToggleUI(InventoryUIInstance);
}


 /*************************************************************************************************
 * ������ ���� Ʈ���ſ� �÷��̾ ������ �� ��Ʈ�ѷ��� ������ �� �Լ��� ȣ��
 * ���� HUD�� ȭ�鿡 ǥ��
 * 
 * @author	������
 * @date	2024-05-26
 * @param	����
 * @return	
 **************************************************************************************************/
void ABasicPlayerController::ShowBossHUD(AActor* EnemyPawn)
{
	BossHUDWidget->InitBar(EnemyPawn);
	BossHUDWidget->SetVisibility(ESlateVisibility::Visible);
}

class UBossHUDWidget* ABasicPlayerController::GetBossHUD()
{
	return BossHUDWidget;
}
