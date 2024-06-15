// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTD_HealthIsHalf.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CainPatternInterface.h"

UBTD_HealthIsHalf::UBTD_HealthIsHalf()
{
	NodeName = TEXT("HealthIsHalf");
}

bool UBTD_HealthIsHalf::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	if (AIPawn->IsFirstPhase() && AIPawn->IsHealthUnderHalf())
	{
		AIPawn->ChangeIntoSecondPhase();
		return true;
	}
	else
	{
		// 그로기 상태
		return AIPawn->GetStatus() == static_cast<int32>(EPattern::GROGGY);
	}
}
