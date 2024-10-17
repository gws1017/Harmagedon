#include "Actor/Item/Armor.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

AArmor::AArmor()
{
}

void AArmor::BeginPlay()
{
	Super::BeginPlay();
}

void AArmor::Equip(EEquipType Type)
{
	Super::Equip(Type);
	InitializeItemData();
	auto Player = Cast<APlayerCharacter>(GetOwnerCharacter());
	Player->EquipArmor(Type,this);
}
