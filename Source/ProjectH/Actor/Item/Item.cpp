#include "Actor/Item/Item.h"
#include "UI/Slot.h"

#include "Data/ItemData.h"

#include "System/MyGameInstance.h"
#include "Global.h"

AItem::AItem()
	:ItemCode(1)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	InitializeItemData();
	
}

void AItem::InitializeItemData()
{
	if (ItemCode >= 1)
	{
		UMyGameInstance* GameInstance = NULL;
		if (GetOwner())
			GameInstance = GetOwner()->GetGameInstance<UMyGameInstance>();
		else
			GameInstance = GetGameInstance<UMyGameInstance>();

		ItemData = GameInstance->GetItemData(ItemCode);
	}
}

void AItem::SetSocketName(EEquipType Type)
{
	//���� ���⺰�� ������ ���� �߰��ؾ��Ѵ�
	// �����̸��� �� �ٸ��� Set�������
	//���� ���� ������ ���� LeftSword RightSword
	//���� ���� ������ ����... LeftShield RightSield

	FString Base = EquipSocket.ToString();
	switch (Type)
	{
	case EEquipType::ET_LeftWeapon:
		EquipSocket = FName(Base + "LSocket");
		break;
	case EEquipType::ET_RightWeapon:
		EquipSocket = FName(Base + "RSocket");
		break;
	case EEquipType::ET_Top:
		EquipSocket = "Top";
		break;
	case EEquipType::ET_Bottom:
		EquipSocket = "Bottom";
		break;
	case EEquipType::ET_Head:
		EquipSocket = "Head";
		break;
	case EEquipType::ET_Shoe:
		EquipSocket = "Shoe";
		break;
	}
}


