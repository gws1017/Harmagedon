// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainDash.generated.h"

 /**************************************************************************************************
 * 대시 태스크
 *
 * @author	조현식
 * @date	2024-05-06
 **************************************************************************************************/
UCLASS()
class PROJECTH_API UBTT_CainDash : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_CainDash();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
