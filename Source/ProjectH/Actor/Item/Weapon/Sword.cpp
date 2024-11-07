#include "Actor/Item/Weapon/Sword.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Interface/HitInterface.h"
#include "Global.h"

ASword::ASword()
{
	ItemCode = 1;
}

void ASword::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WeaponApplyDamage(OtherActor,SweepResult);
}

void ASword::Equip(EEquipType Type)
{
	Super::Equip(Type);

	Cast<APlayerCharacter>(GetOwnerCharacter())->SetWeaponEquipped(EWeaponEquipped::EWE_Sword);
}

void ASword::UnEquip(EEquipType Type)
{
	Super::UnEquip(Type);
}

void ASword::BasicAttack()
{
	Super::BasicAttack();

	CLog::Print("SwordSlash");

	PlayAttackMontage();
}

