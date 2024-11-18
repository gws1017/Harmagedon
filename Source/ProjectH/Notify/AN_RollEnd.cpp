#include "Notify/AN_RollEnd.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

FString UAN_RollEnd::GetNotifyName_Implementation() const
{
	return "RollEnd";
}

void UAN_RollEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->SetMovementNormal();
		if (PlayerCharacter->IsLocking())
			PlayerCharacter->bUseControllerRotationYaw = true;
	}
}