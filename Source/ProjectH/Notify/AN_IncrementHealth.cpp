#include "Notify/AN_IncrementHealth.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

FString UAN_IncrementHealth::GetNotifyName_Implementation() const
{
	return "incrementHealth";
}

void UAN_IncrementHealth::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	auto PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		float HealthPoint = PlayerCharacter->GetHP() + PlayerCharacter->GetMaxHP() * IncrementRate;
		HealthPoint = FMath::Clamp(HealthPoint, 0.0f, PlayerCharacter->GetMaxHP());
		PlayerCharacter->SetHP(HealthPoint);
	}
}