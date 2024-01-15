#include "BehaviorTree/BTT_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Actor/Character/Enemy.h"
#include "Global.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("AttackPhase");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemy* ControlledPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	ControlledPawn->Attack();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}