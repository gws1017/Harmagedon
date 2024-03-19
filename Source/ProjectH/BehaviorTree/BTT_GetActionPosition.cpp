#include "BehaviorTree/BTT_GetActionPosition.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_GetActionPosition::UBTT_GetActionPosition()
{
	NodeName = TEXT("GetActionPosition");
}

EBTNodeResult::Type UBTT_GetActionPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	FVector ActionPosition = ControlledPawn->GetActorLocation();
	ActionPosition += ControlledPawn->GetActorForwardVector() * (ControlledPawn->GetActionRadius() * 2);

	ActionPosition -= ControlledPawn->GetActorForwardVector() * Offset;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector("ActionPosition", ActionPosition);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
