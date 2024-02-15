#include "Slot.h"
#include "UI/Slot.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Component/InventoryComponent.h"
#include "Global.h"

void USlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	//SlotUpdate();
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
}

void USlot::OnClickSlot()
{
	if (SlotClickFunction.IsBound())
	{
		SlotClickFunction.Execute();
	}
	//�ӽñ�� ��������� �ٸ� UI�� ������, �׽�Ʈ��
	//CheckNull(GetOwnerCharacter());
	//GetOwnerCharacter()->GetInventory()->Equip(this);
}

void USlot::AddClickFunction(UObject* InObject, const FName InFunctionName)
{
	if (SlotClickFunction.IsBound() == false)
	{
		SlotClickFunction.BindUFunction(InObject, InFunctionName);
	}
}
