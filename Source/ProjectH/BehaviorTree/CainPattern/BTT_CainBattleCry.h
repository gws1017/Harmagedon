// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainBattleCry.generated.h"

 /**************************************************************************************************
 * 태스크 (배틀 크라이 실행)
 *
 * @author	조현식
 * @date	2024-05-04
 **************************************************************************************************/
UCLASS()
class PROJECTH_API UBTT_CainBattleCry : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_CainBattleCry();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
