#include "UI/StatBarUI.h"
#include "Actor/Character/PlayerCharacter.h"

float UStatBarUI::GetHPPercent()
{
	if (!!OwnerCharacter)
		return (OwnerCharacter->GetHP() / OwnerCharacter->GetMaxHP());

	return 0.0f;
}

float UStatBarUI::GetStaminaPercent()
{
	if (!!OwnerCharacter)
		return (OwnerCharacter->GetStamina() / OwnerCharacter->GetMaxStamina());

	return 0.0f;
}
