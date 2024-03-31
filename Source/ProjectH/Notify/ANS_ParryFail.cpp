#include "Notify/ANS_ParryFail.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

FString UANS_ParryFail::GetNotifyName_Implementation() const
{
	return "ParryFail";
}

void UANS_ParryFail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	CheckNull(player);

	player->SetParryFail(true);
}

void UANS_ParryFail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	CheckNull(player);
	player->SetParryFail(false);
}
