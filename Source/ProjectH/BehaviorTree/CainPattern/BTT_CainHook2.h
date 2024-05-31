// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainHook2.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_CainHook2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_CainHook2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
