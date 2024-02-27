#include "Notify/ANS_CollisionEnable.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Interface/WeaponInterface.h"
#include "UI/Slot.h"
#include "Global.h"

FString UANS_CollisionEnable::GetNotifyName_Implementation() const
{
	return "CollisionEnable";
}

void UANS_CollisionEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IWeaponInterface* owner = Cast<IWeaponInterface>(MeshComp->GetOwner());
	CheckNull(owner);

	AWeapon* weapon = owner->GetWeapon(WeaponEquipType);
	if (!!weapon)
	{
		weapon->Begin_Collision();
	}
}

void UANS_CollisionEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IWeaponInterface* owner = Cast<IWeaponInterface>(MeshComp->GetOwner());
	CheckNull(owner);

	AWeapon* weapon = owner->GetWeapon(WeaponEquipType);
	if (!!weapon)
	{
		weapon->End_Collision();
	}
}
