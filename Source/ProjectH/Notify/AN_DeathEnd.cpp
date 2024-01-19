#include "Notify/AN_DeathEnd.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UAN_DeathEnd::GetNotifyName_Implementation() const
{
	return "Death End";
}
void UAN_DeathEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->DeathEnd();
}