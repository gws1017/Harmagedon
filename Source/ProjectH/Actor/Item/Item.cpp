#include "Actor/Item/Item.h"
#include "Data/ItemData.h"
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
	switch (Type)
	{
	case EEquipType::ET_LeftHand:
		EquipSocket = "LeftHand";
		break;
	case EEquipType::ET_RightHand:
		EquipSocket = "RightHand";
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


