// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_RollEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UAN_RollEnd : public UAnimNotify
{
	GENERATED_BODY()


public:

	FString GetNotifyName_Implementation() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);
};
