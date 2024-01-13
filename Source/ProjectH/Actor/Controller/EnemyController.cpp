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

	UBlackboardComponent* comp = Cast<UBlackboardComponent>(Blackboard);
	CheckNull(comp);
	//UseBlackBoard�� TObjectPtr��ü�� ��������ȯ�� �̷������ �ʴµ�.
	if (UseBlackboard(BlackboardData, comp))
	{
		RunBehaviorTree(BehaviorTree);
	}
}