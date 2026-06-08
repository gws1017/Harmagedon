// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTD_NextPattern.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CainPatternInterface.h"

UBTD_NextPattern::UBTD_NextPattern()
{
	NodeName = TEXT("NextPattern");
}

bool UBTD_NextPattern::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	const bool bAllow = AIPawn->AllowNextPattern();

	// ?? 1?(Punch1/Grab/Punch2) ? BT ??? Inversed ??.
	// Raw ???? ??? ? ? ? ???: ?? true ? raw false.
	if (IsInversed())
	{
		return false;
	}

	// ?? 2?: 1? ??? ?? ???
	return bAllow;
}
