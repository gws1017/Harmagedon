#include "Actor/Item/Weapon/Shield.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

AShield::AShield()
{
	SpecialAttackStaminaCost = 10.f;
}

void AShield::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//방패도 타격이 가능하다
	WeaponApplyDamage(OtherActor,SweepResult);
}

void AShield::BasicAttack()
{
	Super::BasicAttack();

	PlayAttackMontage();
}

void AShield::Block()
{
	Super::Block();
}

void AShield::Parry()
{
	CLog::Print(GetName() + "Special Attack is Parry");
	CheckNull(SpecialAttackMontage);
	auto Player = Cast<APlayerCharacter>(GetOwnerCharacter());
	if (Player)
	{
		if (Player->GetStamina()< SpecialAttackStaminaCost) return;
		Player->DecrementStamina(SpecialAttackStaminaCost);
	}

	GetOwnerCharacter()->PlayAnimMontage(SpecialAttackMontage);

}
