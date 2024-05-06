#include "Actor/Controller/EnemyController.h"
#include "Global.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyController::AEnemyController()
{
	UseBehaviorTree();
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CheckFalse(IsUseBehaviorTree());
	CheckNull(BlackboardData && BehaviorTree);

	RunAI();
}


void AEnemyController::RunAI()
{
	UBlackboardComponent* comp = Blackboard.Get();
	//UseBlackBoard는 TObjectPtr객체가 묵시적변환이 이루어지지 않는듯.
	if (UseBlackboard(BlackboardData, comp))
	{
		RunBehaviorTree(BehaviorTree);
	}
}