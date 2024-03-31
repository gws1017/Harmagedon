#include "Notify/AN_ParryEnd.h"
#include "Actor/Character/PlayerCharacter.h"

FString UAN_ParryEnd::GetNotifyName_Implementation() const
{
	return "ParryEnd";
}

void UAN_ParryEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		
		//패리 처리햇던 것 초기화함
		PlayerCharacter->SetParrySucc(false);
	}
}
