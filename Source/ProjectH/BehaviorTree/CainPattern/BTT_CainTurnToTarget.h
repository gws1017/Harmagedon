// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainTurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_CainTurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_CainTurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
