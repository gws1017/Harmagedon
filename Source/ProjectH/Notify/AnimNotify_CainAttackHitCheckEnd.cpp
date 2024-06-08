// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotify_CainAttackHitCheckEnd.h"
#include "Interface/CainAnimNotifyInterface.h"

void UAnimNotify_CainAttackHitCheckEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		ICainAnimNotifyInterface* AttackPawn = Cast<ICainAnimNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}