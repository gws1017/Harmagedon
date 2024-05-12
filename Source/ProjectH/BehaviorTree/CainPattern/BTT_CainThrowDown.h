// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainThrowDown.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_CainThrowDown : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_CainThrowDown();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
