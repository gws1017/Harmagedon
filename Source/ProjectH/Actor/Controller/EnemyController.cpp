#include "Actor/Controller/EnemyController.h"
#include "Global.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.h"

AEnemyController::AEnemyController()
{
	UseBehaviorTree();
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CheckFalse(IsUseBehaviorTree());
	CheckNull(BlackboardData && BehaviorTree);

	UBlackboardComponent* comp = Blackboard.Get();
	//UseBlackBoard�� TObjectPtr��ü�� ��������ȯ�� �̷������ �ʴµ�.
	if (UseBlackboard(BlackboardData, comp))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AEnemyController::StopBT()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (!!BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
