// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainHook1.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_CainHook1 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_CainHook1();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
