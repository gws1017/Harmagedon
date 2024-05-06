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
	//UseBlackBoard�� TObjectPtr��ü�� ��������ȯ�� �̷������ �ʴµ�.
	if (UseBlackboard(BlackboardData, comp))
	{
		RunBehaviorTree(BehaviorTree);
	}
}