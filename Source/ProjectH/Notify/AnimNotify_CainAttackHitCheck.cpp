// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotify_CainAttackHitCheck.h"
#include "Interface/CainAnimNotifyInterface.h"

void UAnimNotify_CainAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ICainAnimNotifyInterface* AttackPawn = Cast<ICainAnimNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}