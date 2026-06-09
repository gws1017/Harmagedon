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

	// AI 인터페이스 변환
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	if (AIPawn->IsFirstPhase())
	{
		// HP 50% 이하일 때만 2페이즈로 전환
		if (AIPawn->IsHealthUnderHalf())
		{
			AIPawn->ChangeIntoSecondPhase();
			return true;
		}
		return false;
	}
	else
	{
		// 이미 2페이즈 → 그로기 상태 체크
		return AIPawn->GetStatus() == static_cast<int32>(EPattern::GROGGY);
	}
}
