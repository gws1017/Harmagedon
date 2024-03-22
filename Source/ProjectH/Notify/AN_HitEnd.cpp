#include "Notify/AN_HitEnd.h"
#include "Actor/Character/PlayerCharacter.h"

FString UAN_HitEnd::GetNotifyName_Implementation() const
{
	return "HitEnd";
}

void UAN_HitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->SetMovementNormal();
	}
}
