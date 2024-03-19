#include "Notify/AN_AttackEnd.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Character/Enemy.h"
#include "Global.h"

FString UAN_AttackEnd::GetNotifyName_Implementation() const
{
	return "AttackEnd";
}

void UAN_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->End_Attack();
	}
	else
	{
		auto Enemy = Cast<AEnemy>(MeshComp->GetOwner());
		if(Enemy) Enemy->End_Attack();
	}
}