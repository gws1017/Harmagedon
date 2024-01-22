#include "Actor/Item/ExpItem.h"
#include "Actor/Character/PlayerCharacter.h"

void AExpItem::OnInteraction()
{
	PlayerInstance->IncrementExp(Exp);
	Exp = 0;
	
	APickupItem::OnInteraction();
}
