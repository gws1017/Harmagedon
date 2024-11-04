#include "Component/InventoryComponent.h"
#include "System/MyGameInstance.h"
#include "Actor/Item/Item.h"
#include "Actor/Item/EquipmentItem.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Item/Armor.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

UInventoryComponent::UInventoryComponent()
	:Capacity(50.f), CurrentWeight(0.f)
{
	PrimaryComponentTick.bCanEverTick = false;

	/*EquipInfo.Add({ EEquipType::ET_Shoe });
	EquipInfo.Add({ EEquipType::ET_LeftHand });
	EquipInfo.Add({ EEquipType::ET_RightHand });
	EquipInfo.Add({ EEquipType::ET_Top });
	EquipInfo.Add({ EEquipType::ET_Bottom });
	EquipInfo.Add({ EEquipType::ET_Head });
	EquipInfo.Add({ EEquipType::ET_Consumalbe });*/

	
	/*EquipInfo[EEquipType::ET_Shoe].ItemSlots.Reserve(5);
	EquipInfo[EEquipType::ET_LeftHand].ItemSlots.Reserve(5);
	EquipInfo[EEquipType::ET_RightHand].ItemSlots.Reserve(5);
	EquipInfo[EEquipType::ET_Top].ItemSlots.Reserve(5);
	EquipInfo[EEquipType::ET_Bottom].ItemSlots.Reserve(5);
	EquipInfo[EEquipType::ET_Head].ItemSlots.Reserve(5);
	EquipInfo[EEquipType::ET_Consumalbe].ItemSlots.Reserve(5);*/
	
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<FInventoryItem> UInventoryComponent::GetInventoryContents() const
{
	TArray<FInventoryItem> OutArray;
	InventoryContents.GenerateValueArray(OutArray);
	return OutArray;
}

TArray<FInventoryItem> UInventoryComponent::GetInventoryItemsFromItemType(const EItemType Type) const
{
	TArray<FInventoryItem> OutArray;
	for (auto [code, data] : InventoryContents)
	{
		if (data.ItemInfo.ItemType == Type)
		{
			OutArray.Add(data);
		}
	}
	return OutArray;
}

TArray<FInventoryItem> UInventoryComponent::GetInventoryItemsFromEquipType(const EEquipType Type) const
{
	TArray<FInventoryItem> OutArray;
	for (auto [code, data] : InventoryContents)
	{
		if(CheckItemTypeIncludeInEquipType(Type, data.ItemInfo.ItemType))
		{
			OutArray.Add(data);
		}
	}
	return OutArray;
}

void UInventoryComponent::AddItem(const FItemData& ItemData, bool bEquipped)
{
	FInventoryItem* Item = InventoryContents.Find(ItemData.ItemCode);

	//무게 확인
	if (!CheckWeight(ItemData.NumericData.Weight)) return;

	//이미 있는아이템인가?
	if (Item) Item->Count++;
	else InventoryContents.Add({ ItemData.ItemCode ,FInventoryItem(ItemData, bEquipped) });

	CurrentWeight += ItemData.NumericData.Weight;

	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::AddItem(const int64 ItemCode, bool bEquipped)
{
	FInventoryItem* Item = InventoryContents.Find(ItemCode);
	FItemData* Data = GetOwner()->GetGameInstance<UMyGameInstance>()->GetItemData(ItemCode);
	if (Data == nullptr)
		return;
	//무게 확인
	if (!CheckWeight(Data->NumericData.Weight)) return;

	//이미 있는아이템인가?
	if (Item) Item->Count++;
	else
	{
		FInventoryItem NewItem = FInventoryItem(*Data, bEquipped);
		USlot* Slot = CreateWidget<USlot>(GetWorld(), SlotClass);
		if (Slot)
		{
			Slot->SetItemCode(ItemCode);
			ItemSlots.Add(Slot);
			OnInventoryUpdated.AddUFunction(Slot, "SlotUpdate");
			NewItem.Slot = Slot;
			InventoryContents.Add({ ItemCode ,NewItem });

		}
		
	}
	CurrentWeight += Data->NumericData.Weight;
	
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::RemoveItem(const int64 ItemCode)
{
	auto item = InventoryContents.Find(ItemCode);
	if (item->Count <= 1)
		InventoryContents.Remove(ItemCode);
	else
		item->Count--;

	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::ClearInventory()
{
	InventoryContents.Empty();
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::UpdateSlot()
{
	for (auto [code, item] : InventoryContents)
	{
		item.Slot->SetItemCode(code);
	}
	OnInventoryUpdated.Broadcast();

}

//void UInventoryComponent::UpdateEquipItem()
//{
//	for (auto [Type, Slots] : EquipInfo)
//	{
//		if (Slots.ItemSlots.IsEmpty()) continue;
//		//슬롯에 들어가있는 장비 중 선택 슬롯의 장비를 장착한다.
//		auto Slot = EquipInfo[Type].ItemSlots[Slots.SelectSlot];
//		if (Slot && Slot->bEquipped)
//		{
//			GetCDOItem(Slot->ItemInfo.AssetData.ItemClass)->Equip(Type);
//		}
//		
//	}
//}
//
//void UInventoryComponent::EquipInSlot(const int64 ItemCode, int32 SlotNumber)
//{
//	FInventoryItem* Data = InventoryContents.Find(ItemCode);
//	CheckNull(Data);
//	EEquipType Type = Data->ItemInfo.EquipType;
//
//	FInventoryItem* Slot = EquipInfo[Type].ItemSlots[SlotNumber];
//	if(Slot != nullptr)
//	{
//		(Slot)->bEquipped = false;
//	}
//
//	Slot = Data;
//	Data->bEquipped = true;
//}
//
//void UInventoryComponent::UnEquipInSlot(EEquipType Type, int32 SlotNumber)
//{
//	auto Slot = EquipInfo[Type].ItemSlots[SlotNumber];
//	Slot->bEquipped = false;
//	Slot = nullptr;
//}

void UInventoryComponent::Equip(USlot* SelectSlot, USlot* InvenSlot, AItem* ItemInstance)
{
	//UI에서 전달받은 장비창 슬롯정보로 아이템을 장비한다
	if (InvenSlot == nullptr)
	{
		CLog::Log("Equip State Error");
		return;
	}

	auto Player = Cast<APlayerCharacter>(GetOwner());
	if (!!SelectSlot && SelectSlot->bEquipped == true)
	{
		QuickUnEquip(SelectSlot);
	}
	if (!!InvenSlot && InvenSlot->bEquipped == true)
	{
		QuickUnEquip(InvenSlot);
	}

	SelectSlot->bEquipped = true;
	auto InvenData = InventoryContents.Find(InvenSlot->ItemInfo.ItemCode);
	InvenData->Slot = SelectSlot;
	InvenData->bEquipped = true;

	auto Type = SelectSlot->EquipType;
	AEquipmentItem* Instance = Cast<AEquipmentItem>(GetCDOItem(InvenSlot->ItemInfo.AssetData.ItemClass));
	Instance->SetOwner(Player);

	AArmor* ArmorItem = Cast<AArmor>(Instance);
	//무기나 장비일 경우 실제 액터를 생성한다
	if (Type == EEquipType::ET_LeftWeapon || Type == EEquipType::ET_RightWeapon)
	{
		Instance = Cast<AWeapon>(AItem::Spawn<AItem>(GetWorld(),
			InvenSlot->ItemInfo.AssetData.ItemClass, Cast<ACharacter>(GetOwner())));
		Player->SetWeapon(Type, Cast<AWeapon>(Instance));
		Player->SetCapture(Instance, true);
		Player->ActiveWeapon = Cast<AWeapon>(Instance);
	}
	
	if (!!ItemInstance)
		ItemInstance = Instance;

	SelectSlot->ItemInstance = Instance;
	if (ArmorItem)
		ArmorItem->Equip(Type);
	else
		Player->Equip(SelectSlot->EquipType,Instance);
}

void UInventoryComponent::UnEquip(USlot* EquipSlot)
{
	//GetEquipment(EEquipType)로 현재 장착중인 장비의 인스턴스를 가져오자
	if (EquipSlot == nullptr)
	{
		CLog::Log("Equip State Error");
		return;
	}
	EquipSlot->bEquipped = false;

	//아이템데이터에서 장착 해제 
	auto InvenData = InventoryContents.Find(EquipSlot->ItemInfo.ItemCode);
	InvenData->Slot = nullptr;
	InvenData->bEquipped = false;
	
	auto Player = Cast<APlayerCharacter>(GetOwner());
	Player->ActiveWeapon = Cast<AWeapon>(EquipSlot->ItemInstance);
	Player->UnEquip(EquipSlot->EquipType);
	EquipSlot->ItemInstance = nullptr;
	
}

void UInventoryComponent::QuickUnEquip(USlot* EquipSlot)
{
	auto Player = Cast<APlayerCharacter>(GetOwner());
	auto TargetSlot = GetItemData(EquipSlot->ItemInfo.ItemCode).Slot;
	Player->QuickUnEquip(Cast<AWeapon>(TargetSlot->ItemInstance));
	UnEquip(TargetSlot);
	TargetSlot->ClearSlot();
}

void UInventoryComponent::Use(const int64 ItemCode)
{
	FInventoryItem* Data = InventoryContents.Find(ItemCode);
	CheckNull(Data);
	GetCDOItem(Data->ItemInfo.AssetData.ItemClass)->Use();
}

bool UInventoryComponent::CheckWeight(float weight)
{
	if (CurrentWeight + weight <= Capacity)
		return true;
	//UI를 띄우거나 메시지를 띄우기
	CLog::Print("인벤토리 중량을 초과했습니다.");
	return false;
}

bool UInventoryComponent::CheckItemTypeIncludeInEquipType(const EEquipType EquipType, const EItemType ItemType) const
{
	CLog::Log("Type Check");

	bool ret = false;
	switch (EquipType)
	{
		case EEquipType::ET_RightWeapon:
		case EEquipType::ET_LeftWeapon:
			if (ItemType == EItemType::IT_MagicalWeapon ||
				ItemType == EItemType::IT_PhysicalWeapon)
				ret = true;
			else
				ret = false;
			break;
		case EEquipType::ET_Top:
			if (ItemType == EItemType::IT_Top)
				ret = true;
			else ret = false;
			break;
		case EEquipType::ET_Head:
			if (ItemType == EItemType::IT_Head)
				ret = true;
			else ret = false;
			break;
		case EEquipType::ET_Shoe:
			if (ItemType == EItemType::IT_Shoe)
				ret = true;
			else ret = false;
			break;
		case EEquipType::ET_Hand:
			if (ItemType == EItemType::IT_Hand)
				ret = true;
			else ret = false;
			break;
		case EEquipType::ET_Bottom:
			if (ItemType == EItemType::IT_Bottom)
				ret = true;
			else ret = false;
			break;
		case EEquipType::ET_Cousumable:
			if (ItemType == EItemType::IT_Cousumable)
				ret = true;
			else ret = false;
			break;
		default:
			ret = false;
			break;
	}
	return ret;
}

FInventoryItem UInventoryComponent::GetItemDataFromSlot(USlot* Slot)
{
	for (const auto& [code, item] : InventoryContents)
	{
		if (item.Slot == Slot)
		{

			return item;
		}
	}
	return FInventoryItem{};
}

AItem* UInventoryComponent::GetCDOItem(TSubclassOf<AItem> Item) const
{
	if (Item)
	{
		AItem* CDO = Item.GetDefaultObject();
		CDO->SetOwner(GetOwner());
		return CDO;
	}
	else
	{
		CLog::Log("Item Class is Null!");
		return nullptr;
	}
}


