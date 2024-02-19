#include "Notify/ANS_Equip.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UANS_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UANS_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IICharacter* CharacterInstance = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(CharacterInstance);
	CheckNull(CharacterInstance->GetWeapon());
	auto Player = Cast<APlayerCharacter>(CharacterInstance);
	if (Player)
	{
		Player->ActiveWeapon->Begin_Equip();
		return;
	}
	CharacterInstance->GetWeapon()->Begin_Equip();
}

void UANS_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* CharacterInstance = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(CharacterInstance);
	CheckNull(CharacterInstance->GetWeapon());
	auto Player = Cast<APlayerCharacter>(CharacterInstance);
	if (Player)
	{
		Player->ActiveWeapon->End_Equip();
		return;
	}
	CharacterInstance->GetWeapon()->End_Equip();
}

