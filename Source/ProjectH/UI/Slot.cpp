#include "UI/Slot.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Component/InventoryComponent.h"
#include "Global.h"

void USlot::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void USlot::SlotUpdate()
{
	CheckNull(GetOwnerCharacter());
	FInventoryItem Data = GetOwnerCharacter()->GetInventory()->GetItemData(ItemInfo.ItemCode);
	ItemInfo = Data.ItemInfo;
	Count = Data.Count;
	bEquipped = Data.bEquipped;
}

void USlot::SlotUpdateFromData(const FInventoryItem Data)
{
	ItemInfo = Data.ItemInfo;
	Count = Data.Count;
	bEquipped = Data.bEquipped;
	InitializeSlotInfo();
}

void USlot::SlotUpdateFromSlot(const USlot* Data)
{
	ItemInfo = Data->ItemInfo;
	Count = Data->Count;
	bEquipped = Data->bEquipped;
	InitializeSlotInfo();
}

void USlot::OnClickSlot()
{
	if (SlotClickFunction.IsBound())
	{
		SlotClickFunction.Execute(this);
	}
}

void USlot::OnHoverSlot()
{
	if (SlotHoverFunction.IsBound())
	{
		SlotHoverFunction.Execute(this);
	}
}

void USlot::AddClickFunction(UObject* InObject, const FName InFunctionName)
{
	SlotClickFunction.Clear();
	SlotClickFunction.BindUFunction(InObject, InFunctionName);
	
}

void USlot::AddHoverFunction(UObject* InObject, const FName InFunctionName)
{
	SlotHoverFunction.Clear();
	SlotHoverFunction.BindUFunction(InObject, InFunctionName);
}
