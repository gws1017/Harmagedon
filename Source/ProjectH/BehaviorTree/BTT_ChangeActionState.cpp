#include "BehaviorTree/BTT_ChangeActionState.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"

UBTT_ChangeActionState::UBTT_ChangeActionState()
{
	NodeName = TEXT("ChangeActionState");
}

EBTNodeResult::Type UBTT_ChangeActionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	ControlledPawn->SetActionState(Action);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
