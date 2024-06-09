// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTD_PreventSamePattern.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CainPatternInterface.h"
#include "Data/CainAIKey.h"

UBTD_PreventSamePattern::UBTD_PreventSamePattern()
{
}

bool UBTD_PreventSamePattern::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	int CurrentRandomNumber = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_RANDOMNUMBER);


	return AIPawn->GetPrevRandomNumber() != CurrentRandomNumber;
}
