#include "UI/InventoryUI.h"
#include "UI/Slot.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

TArray<FInventoryItem> UInventoryUI::GetInventoryItemsFromEquipType(const EEquipType Type)
{
	return  GetOwnerCharacter()->GetInventory()->GetInventoryItemsFromEquipType(Type);
}

TArray<FInventoryItem> UInventoryUI::GetInventoryItemsFromItemType(const EItemType Type)
{
	return GetOwnerCharacter()->GetInventory()->GetInventoryItemsFromItemType(Type);
}
