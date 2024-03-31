#include "Notify/ANS_ParryTime.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

FString UANS_ParryTime::GetNotifyName_Implementation() const
{
	return "ParryTime";
}

void UANS_ParryTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	CheckNull(player);

	player->SetCanParryed(bCanParryed);
	player->SetInvincible(!bCanDamaged);
	
}

void UANS_ParryTime::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	CheckNull(player);

	//�и� �����ȉ����� ������ ���׹̳���ŭ �Ҹ�
	//8������ - 0
	//4������ - 20%
	//���� - 40%
	
	player->SetCanParryed(false);
	player->SetInvincible(false);

}
