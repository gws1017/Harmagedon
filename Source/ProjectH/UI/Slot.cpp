#include "Slot.h"
#include "UI/Slot.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Component/InventoryComponent.h"
#include "Global.h"

void USlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	SlotUpdate();
	//����Ŭ���ϸ� ���������ǰ��ؾ���
}

void USlot::SlotUpdate()
{
	CheckNull(GetOwnerCharacter());
	FInventoryItem Data = GetOwnerCharacter()->GetInventory()->GetItemData(ItemInfo.ItemCode);
	ItemInfo = Data.ItemInfo;
	Count = Data.Count;
	bEquipped = Data.bEquipped;
}

void USlot::OnClickSlot()
{
	//�ӽñ�� ��������� �ٸ� UI�� ������, �׽�Ʈ��
	CheckNull(GetOwnerCharacter());
	GetOwnerCharacter()->GetInventory()->Equip(this);
}
