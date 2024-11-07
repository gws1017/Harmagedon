#include "Actor/Item/EquipmentItem.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Animation/AnimMontage.h"

AEquipmentItem::AEquipmentItem()
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void AEquipmentItem::BeginPlay()
{
	Super::BeginPlay();

}

ACharacter* AEquipmentItem::GetOwnerCharacter()
{
	if (OwnerCharacter == nullptr)
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	return OwnerCharacter; 
}

void AEquipmentItem::Equip(EEquipType Type)
{

	CheckTrue(bEquipped);//이미 착용중이면 함수 종료
	CheckTrue(bEquipping);//무기 꺼내는중이여도 종료
	EquipType = Type;
	if (OwnerCharacter == nullptr)
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	SetSocketName(Type);
	bEquipping = true;

}

void AEquipmentItem::UnEquip(EEquipType Type)
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);//무기 넣는중이여도 종료

	bEquipping = true;
}

void AEquipmentItem::Begin_Equip()
{
	CheckTrue(bEquipped)
	bEquipped = true;
	CheckNull(OwnerCharacter);
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), EquipSocket);
}

void AEquipmentItem::End_Equip()
{
	bEquipping = false;
}

void AEquipmentItem::Begin_UnEquip()
{
	bEquipped = false;
}

void AEquipmentItem::End_UnEquip()
{
	bEquipped = false;
	bEquipping = false;
	auto Player = Cast<APlayerCharacter>(GetOwnerCharacter());
	if (Player)
	{
		auto& EquippedMap = Player->GetEquipmentMap();
		EquippedMap[EquipType] = nullptr;
		Player->EmptyWeapon();
	}
	Destroy();
}
