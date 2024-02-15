#include "UI/InventoryUI.h"
#include "UI/Slot.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

TArray<FInventoryItem> UInventoryUI::GetInventoryItemsFromType(const EItemType Type)
{
	return GetOwnerCharacter()->GetInventory()->GetInventoryItemsFromType(Type);
}
