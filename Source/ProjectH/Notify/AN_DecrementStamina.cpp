#include "Notify/AN_DecrementStamina.h"
#include "Actor/Character/NormalMonster.h"

FString UAN_DecrementStamina::GetNotifyName_Implementation() const
{
	return "DecrementStamina";
}

void UAN_DecrementStamina::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto Enemy = Cast<ANormalMonster>(MeshComp->GetOwner());

	if (Enemy)
	{
		Enemy->DecrementStamina();
	}

}
