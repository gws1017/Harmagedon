// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateActionState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTS_UpdateActionState : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTS_UpdateActionState();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};