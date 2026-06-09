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

	// 콤보 1타(Punch1/Grab/Punch2) — BT 에디터에서 Inversed 체크됨.
	// CalculateRawConditionValue 반환값은 엔진이 IsInversed()로 한 번 더 뒤집음:
	//   최종 true  → raw false
	//   최종 false → raw true
	// idle(bAllow=true)에서 1타를 열려면 최종 true 필요 → raw false 고정.
	// (이전에 raw true를 반환하면 Inversed에서 최종 false가 되어 55<70 Hook2 등 전부 막힘)
	if (IsInversed())
	{
		return false;
	}

	// 콤보 2타(Hook1/ThrowAway/Uppercut/Hook2): 1타 몽타주 종료 후에만
	return bAllow;
}
