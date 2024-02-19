#include "UI/SelectionUI.h"
#include "UI/Slot.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Component/InventoryComponent.h"
#include "Global.h"

void USelectionUI::OnThrow(USlot* SlotData)
{
	CheckNull(Slot);
	GetOwnerCharacter()->GetInventory()->RemoveItem(SlotData->ItemInfo.ItemCode);
}
