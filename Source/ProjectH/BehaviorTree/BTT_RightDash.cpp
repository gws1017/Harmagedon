// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTT_RightDash.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CainAIKey.h"

UBTT_RightDash::UBTT_RightDash()
{
}

EBTNodeResult::Type UBTT_RightDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 폰 가져오기
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	//AI 폰으로 변환
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 델리게이트에 함수 등록
	FCainMontageFinished OnFinished;
	OnFinished.BindLambda(
		[&]()
		{
			// InProgress에서 이 태스크를 성공으로 마무리 짓고 다음 노드로 이동
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->SetMontageFinDelegate(OnFinished);

	// 공격
	AIPawn->PlayMontageByAI(EMontages::RIGHTDASH);

	return EBTNodeResult::InProgress;
}
