// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainThrowRock.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_CainThrowRock : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_CainThrowRock();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};