#include "BehaviorTree/BTT_GetPatrolPosition.h"
#include "Actor/Character/NormalMonster.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_GetPatrolPosition::UBTT_GetPatrolPosition()
{
	NodeName = TEXT("GetPatrolPosition");
}

EBTNodeResult::Type UBTT_GetPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;
	
	//Patrol이 지정된게아니라면 아무런 행동도 하지않는다
	if (ControlledPawn->IsPatrol() == false)
	{
		return EBTNodeResult::Failed;
	}

	FVector NextPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector("PatrolPosition");
	
	if (FVector::PointsAreSame(NextPatrolPosition, ControlledPawn->GetPatrolPosition()))
	{
		NextPatrolPosition = ControlledPawn->GetSpawnLocation();
	}
	else 
	{
		NextPatrolPosition = ControlledPawn->GetPatrolPosition();
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolPosition", NextPatrolPosition);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
