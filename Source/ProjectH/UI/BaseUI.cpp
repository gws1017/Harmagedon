#include "UI/BaseUI.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

void UBaseUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!OwnerCharacter)
		OwnerCharacter = GetOwningPlayerPawn<APlayerCharacter>();
	if (!OwnerCharacter)
		CLog::Log("UI Owner InValidError");
}
