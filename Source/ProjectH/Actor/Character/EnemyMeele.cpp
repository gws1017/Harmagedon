#include "Actor/Character/EnemyMeele.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Data/ItemData.h"
#include "Global.h"

#include "Components/SphereComponent.h"

AEnemyMeele::AEnemyMeele()
{
	MaxHP = 15;
	HP = MaxHP;
	Exp = 20;

	AgroSphere->InitSphereRadius(300.f);
	CombatSphere->InitSphereRadius(75.f);
}

void AEnemyMeele::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyMeele::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			SetAlerted(true);
			if (!!WeaponClass)
				WeaponInstance = AWeapon::Spawn<AWeapon>(GetWorld(), WeaponClass, this);
			WeaponInstance->Equip(EEquipType::ET_RightHand);
			CombatTarget = player;
			//player->SetTarget(this);
		}
	}
}

void AEnemyMeele::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			CombatTarget = nullptr;
			WeaponInstance->UnEquip(EEquipType::ET_RightHand);
			//player->SetTarget(nullptr);
		}
	}
}

void AEnemyMeele::Attack()
{
	AEnemy::Attack();
	CheckNull(AttackMontage);
	if (bAlerted && !WeaponInstance->GetEquipped())
		WeaponInstance->Equip(EEquipType::ET_RightHand);
	PlayAnimMontage(AttackMontage);
}
