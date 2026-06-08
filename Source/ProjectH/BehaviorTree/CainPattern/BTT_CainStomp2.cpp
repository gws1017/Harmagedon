// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTT_CainStomp2.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CainAIKey.h"

UBTT_CainStomp2::UBTT_CainStomp2()
{
}

EBTNodeResult::Type UBTT_CainStomp2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		const float Dist = BB->GetValueAsFloat(BBKEY_DISTANCE);
		if (Dist >= 400.f)
		{
			return EBTNodeResult::Failed;
		}
	}

	auto bTaskCompleted = MakeShared<bool>(false);
	// 델리게이트에 함수 등록
	FCainMontageFinished OnFinished;
	OnFinished.BindLambda([&OwnerComp, bTaskCompleted, this]()
	{
		if (*bTaskCompleted) return;
		*bTaskCompleted = true;
		// InProgress에서 이 태스크를 성공으로 마무리 짓고 다음 노드로 이동
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});
	AIPawn->SetMontageFinDelegate(OnFinished);

	// 공격
	AIPawn->PlayMontageByAI(EPattern::STOMP2);
	
	// 타이머로 애니메이션 종료 처리
	APawn* PawnForTimer = ControllingPawn;
	UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;
	if (UWorld* World = ControllingPawn->GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, [AIPawn, PawnForTimer, OwnerCompPtr, bTaskCompleted, this]()
		{
			if (!IsValid(PawnForTimer)) return;

			ICainPatternInterface* Pawn = Cast<ICainPatternInterface>(PawnForTimer);
			if (Pawn && !Pawn->AllowNextPattern())
			{
				Pawn->StopAnim();
			}
			else if (!*bTaskCompleted && OwnerCompPtr && IsValid(OwnerCompPtr))
			{
				*bTaskCompleted = true;
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
			}
		}, 4.0f, false);
	}

	return EBTNodeResult::InProgress;
}
