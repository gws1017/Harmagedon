// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Controller/CainController.h"
#include "BehaviorTree/BehaviorTree.h"

ACainController::ACainController()
{
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
