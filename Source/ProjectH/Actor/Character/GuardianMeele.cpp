#include "Actor/Character/GuardianMeele.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Global.h"

#include "Components/SphereComponent.h"

AGuardianMeele::AGuardianMeele()
{
	MaxHP = 15;
	HP = MaxHP;
	Exp = 20;

	AgroSphere->InitSphereRadius(300.f);
	ActionSphere->InitSphereRadius(75.f);

}

void AGuardianMeele::BeginPlay()
{
	Super::BeginPlay();

	if (!!WeaponClass)
		WeaponInstance = AWeapon::Spawn<AWeapon>(GetWorld(), WeaponClass, this);

	GetMesh()->HideBoneByName("weapon_sword_l", EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName("weapon_sword_r", EPhysBodyOp::PBO_None);
	WeaponInstance->Begin_Equip();
}

float AGuardianMeele::GetDamage(const EEquipType Type) const
{
	return WeaponInstance->GetDamage(); 
}

void AGuardianMeele::Attack()
{
	AEnemy::Attack();
	CheckNull(AttackMontage);
	//if (bAlerted && !WeaponInstance->GetEquipped())
		//WeaponInstance->Equip(EEquipType::ET_RightWeapon);
	PlayAnimMontage(AttackMontage);
}

void AGuardianMeele::Disappear()
{
	AEnemy::Disappear();
	if (WeaponInstance)
		WeaponInstance->End_UnEquip();
}
