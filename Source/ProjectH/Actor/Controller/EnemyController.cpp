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
	//UseBlackBoard는 TObjectPtr객체가 묵시적변환이 이루어지지 않는듯.
	if (UseBlackboard(BlackboardData, comp))
	{
		RunBehaviorTree(BehaviorTree);
	}
}