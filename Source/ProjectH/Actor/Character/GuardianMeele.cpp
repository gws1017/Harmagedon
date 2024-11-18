#include "Actor/Character/GuardianMeele.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Global.h"

#include "Components/SphereComponent.h"

AGuardianMeele::AGuardianMeele()
{
	MaxHP = 220;
	HP = MaxHP;
	Exp = 120;

	DetectRadius = 500.f;
	ActionRadius = 300.f;

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
	return WeaponInstance->GetPhysicalDamage(); 
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

void AGuardianMeele::DecrementStamina()
{
	if (WeaponInstance)
		CurrentStamina -= WeaponInstance->GetStaminaCost();
}
