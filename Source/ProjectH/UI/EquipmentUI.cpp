#include "UI/EquipmentUI.h"
#include "Global.h"

void UEquipmentUI::AddEquipSlot(const EEquipType Type, USlot* EquipSlot)
{
	if (SlotMap.Contains(Type))
	{
		SlotMap[Type].SlotArray.Add(EquipSlot);
	}
	else
	{
		FSlotArray arr;
		arr.SlotArray.Add(EquipSlot);
		SlotMap.Add({ Type,arr });
	}
}
