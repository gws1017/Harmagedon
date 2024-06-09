// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTD_3Hit.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CainPatternInterface.h"

UBTD_3Hit::UBTD_3Hit()
{
}

bool UBTD_3Hit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 폰 가져오기
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	// AI 폰으로 변환
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	return AIPawn->GetHitCount() >= 3;
}
