#include "BehaviorTree/BTT_ChangeSpeed.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"
UBTT_ChangeSpeed::UBTT_ChangeSpeed()
{
	NodeName = TEXT("ChangeSpeed");
}

EBTNodeResult::Type UBTT_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	ControlledPawn->SetChase(Speed);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
