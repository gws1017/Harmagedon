// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CainAttackHitCheck2.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UAnimNotify_CainAttackHitCheck2 : public UAnimNotify
{
	GENERATED_BODY()
	
	protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
