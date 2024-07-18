#include "UI/TitleUI.h"
#include "System/MySaveGame.h"
#include "System/MyGameInstance.h"
#include "Global.h"

void UTitleUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
		if (LoadGameInstance)
			bCanContinue = true;
		else
			bCanContinue = false;

}

void UTitleUI::OnClickNewGameButton()
{
	auto GameInstance = GetGameInstance<UMyGameInstance>();
	GameInstance->SetIsNewGame(true);
	UGameplayStatics::OpenLevel(this, TUTORIAL_LEVEL);
}

void UTitleUI::OnClickContinueButton()
{
	auto GameInstance = GetGameInstance<UMyGameInstance>();
	GameInstance->SetIsNewGame(false);
	UGameplayStatics::OpenLevel(this, TUTORIAL_LEVEL);
}
