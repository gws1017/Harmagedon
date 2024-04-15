#include "Actor/Item/Weapon/Shield.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

void AShield::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AShield::Parry()
{
	CLog::Print(GetName() + "Special Attack is Parry");
	CheckNull(SpecialAttackMontage);
	auto Player = Cast<APlayerCharacter>(OwnerCharacter);
	if (Player)
	{
		if (Player->GetStamina()< SpecialAttackStaminaCost) return;
		Player->DecrementStamina(SpecialAttackStaminaCost);
	}

	OwnerCharacter->PlayAnimMontage(SpecialAttackMontage);

}
