#include "UI/BaseUI.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Global.h"

void UBaseUI::NativePreConstruct()
{
	Super::NativePreConstruct();
}

APlayerCharacter* UBaseUI::GetOwnerCharacter()
{
	if (!OwnerCharacter)
		OwnerCharacter = GetOwningPlayerPawn<APlayerCharacter>();
		
	return OwnerCharacter;
}

ABasicPlayerController* UBaseUI::GetOwnerController()
{
	if (!OwnerController)
		OwnerController = GetOwningPlayer<ABasicPlayerController>();

	return OwnerController;
}
