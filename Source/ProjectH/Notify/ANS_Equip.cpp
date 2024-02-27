#include "Notify/ANS_Equip.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Interface/WeaponInterface.h"
#include "Global.h"

FString UANS_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UANS_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IWeaponInterface* owner = Cast<IWeaponInterface>(MeshComp->GetOwner());
	CheckNull(owner);
	CheckNull(owner->GetWeapon());
	auto Player = Cast<APlayerCharacter>(owner);
	if (Player)
	{
		Player->ActiveWeapon->Begin_Equip();
		return;
	}
	owner->GetWeapon()->Begin_Equip();
}

void UANS_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IWeaponInterface* owner = Cast<IWeaponInterface>(MeshComp->GetOwner());
	CheckNull(owner);
	CheckNull(owner->GetWeapon());
	auto Player = Cast<APlayerCharacter>(owner);
	if (Player)
	{
		Player->ActiveWeapon->End_Equip();
		return;
	}
	owner->GetWeapon()->End_Equip();
}

