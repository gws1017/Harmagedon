#include "Notify/ANS_InvincibleFrame.h"
#include "Actor/Character/PlayerCharacter.h"

#include "Global.h"

FString UANS_InvincibleFrame::GetNotifyName_Implementation() const
{
	return "InvincibleFrame";
}

void UANS_InvincibleFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	CheckNull(player);
	player->SetInvincible(true);
}

void UANS_InvincibleFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	CheckNull(player);
	player->SetInvincible(false);
}
