#include "Notify/AN_NextAttack.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

FString UAN_NextAttack::GetNotifyName_Implementation() const
{
	return "NextAttack";
}

void UAN_NextAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	APlayerCharacter* owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	CheckNull(owner);
	owner->AttackCombo();
	CLog::Print("NextAttack");
}
