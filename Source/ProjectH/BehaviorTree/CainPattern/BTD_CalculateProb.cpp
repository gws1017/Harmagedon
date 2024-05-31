// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTD_CalculateProb.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CainPatternInterface.h"
#include "Math/UnrealMathUtility.h" // FMath 헤더 포함
#include "Data/CainAIKey.h"

UBTD_CalculateProb::UBTD_CalculateProb()
{
}

bool UBTD_CalculateProb::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	int32 AttackProb = FMath::RandRange(1, 100);
	int32 SkillProb = FMath::RandRange(1, 100);
	int32 DashProb = FMath::RandRange(1, 100);

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKPROB, AttackProb);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_SKILLPROB, SkillProb);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_DASHPROB, DashProb);


	return true;
}
