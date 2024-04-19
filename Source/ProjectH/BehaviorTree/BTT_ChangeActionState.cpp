#include "BehaviorTree/BTT_ChangeActionState.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ChangeActionState::UBTT_ChangeActionState()
{
	NodeName = TEXT("ChangeActionState");
}

EBTNodeResult::Type UBTT_ChangeActionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;
	if (Action == EMonsterAction::EMA_StandBy)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsRanged", false);
	}
	ControlledPawn->SetActionState(Action);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
