#include "Actor/Item/Item.h"
#include "UI/Slot.h"
#include "Global.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::SetSocketName(EEquipType Type)
{
	//추후 무기별로 소켓을 따로 추가해야한다
	// 소켓이름도 다 다르게 Set해줘야함
	//직검 왼쪽 오른쪽 소켓 LeftSword RightSword
	//방패 왼쪽 오른쪽 소켓... LeftShield RightSield
	switch (Type)
	{
	case EEquipType::ET_LeftWeapon:
		EquipSocket = "LeftWeapon";
		break;
	case EEquipType::ET_RightWeapon:
		EquipSocket = "RightWeapon";
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


