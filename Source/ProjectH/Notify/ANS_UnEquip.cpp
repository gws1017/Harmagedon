#include "Notify/ANS_UnEquip.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UANS_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UANS_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->Begin_UnEquip();
}

void UANS_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->End_UnEquip();
}