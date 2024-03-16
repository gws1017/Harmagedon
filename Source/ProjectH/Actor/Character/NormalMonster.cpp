#include "Actor/Character/NormalMonster.h"
#include "Actor/Character/PlayerCharacter.h"

void ANormalMonster::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			SetAlerted(true);
			//
			//WeaponInstance->Equip(EEquipType::ET_RightWeapon);
			CombatTarget = player;
			//player->SetTarget(this);
		}
	}
}

void ANormalMonster::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			CombatTarget = nullptr;
			//WeaponInstance->UnEquip(EEquipType::ET_RightWeapon);
			//player->SetTarget(nullptr);
		}
	}
}
