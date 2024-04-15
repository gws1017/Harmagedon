#include "Actor/Item/Weapon/Sword.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Interface/HitInterface.h"
#include "Global.h"

void ASword::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�ڱ��ڽ� ����
	IgnoreActors.AddUnique(Owner);

	//���� ���� �׷��̸� ������ �ʴ´�
	if (IsSameTagWithTarget(OtherActor, "Enemy")) return;
	if (IsSameTagWithTarget(OtherActor, "Player")) return;

	//���� ����� ����
	CreateField(GetActorLocation());

	if (!!OtherActor && !IgnoreActors.Contains(OtherActor))
	{
		IHitInterface* other = Cast<IHitInterface>(OtherActor);

		CheckNull(other);
		float AdditionalDamage = 0.f;
		if (Owner->ActorHasTag("Player"))
			AdditionalDamage = Cast<APlayerCharacter>(Owner)->GetStrDamage();

		IgnoreActors.AddUnique(OtherActor);

		other->Hit(GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, Damage + AdditionalDamage, WeaponInstigator, Owner, DamageTypeClass);
	}
}

void ASword::BasicAttack()
{
	Super::BasicAttack();

	CheckNull(AttackMontage);
	CLog::Print("SwordSlash");

	auto Player = Cast<APlayerCharacter>(OwnerCharacter);
	if (!Player)
	{
		OwnerCharacter->PlayAnimMontage(AttackMontage);
	}
	else
	{
		Player->PlayAttackMontage(EquipType);
	}
}
