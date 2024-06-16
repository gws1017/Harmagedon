#include "Actor/Item/Weapon/Sword.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Interface/HitInterface.h"
#include "Global.h"

void ASword::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WeaponApplyDamage(OtherActor,SweepResult);
}

void ASword::BasicAttack()
{
	Super::BasicAttack();

	CLog::Print("SwordSlash");

	PlayAttackMontage();
}
