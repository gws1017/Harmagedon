#include "Notify/ANS_CollisionEnable.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UANS_CollisionEnable::GetNotifyName_Implementation() const
{
	return "CollisionEnable";
}

void UANS_CollisionEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	AWeapon* weapon = owner->GetWeapon();
	if (!!weapon)
	{
		weapon->Begin_Collision();
	}
}

void UANS_CollisionEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	AWeapon* weapon = owner->GetWeapon();
	if (!!weapon)
	{
		weapon->End_Collision();
	}
}
