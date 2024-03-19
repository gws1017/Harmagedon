#include "BehaviorTree/BTT_GetStandByPosition.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_GetStandByPosition::UBTT_GetStandByPosition()
{
	NodeName = TEXT("GetStandByPosition");
}

EBTNodeResult::Type UBTT_GetStandByPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	FVector StandByPosition = ControlledPawn->GetCombatTargetLocation();
	StandByPosition += -ControlledPawn->GetActorForwardVector() * (ControlledPawn->GetActionRadius() * 2);

	StandByPosition -= -ControlledPawn->GetActorForwardVector() * Offset;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector("StandByPosition", StandByPosition);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
