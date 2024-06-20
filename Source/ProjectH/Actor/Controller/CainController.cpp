// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Controller/CainController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CainAIKey.h"

ACainController::ACainController()
{
}

void ACainController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BlackboardData, BlackboardPtr))
	{
		// 홈 포지션에 값 설정함
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BehaviorTree);
		ensure(RunResult);
	}
}

void ACainController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ACainController::OnPossess(APawn* InPawn)
{
	AEnemyController::OnPossess(InPawn);
}
