#include "BehaviorTree/BTT_LockOnTarget.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"

UBTT_LockOnTarget::UBTT_LockOnTarget()
{
	NodeName = TEXT("LockOnTarget");
}

EBTNodeResult::Type UBTT_LockOnTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	if(ControlledPawn->GetTarget())
		ControlledPawn->SetLockOn(bTargetLock);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
