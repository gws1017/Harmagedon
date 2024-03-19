#include "BehaviorTree/BTT_GetAttackPosition.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTT_GetAttackPosition::UBTT_GetAttackPosition()
{
	NodeName = TEXT("GetAttackPosition");
}

EBTNodeResult::Type UBTT_GetAttackPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	FVector AttackPosition = ControlledPawn->GetCombatTargetLocation();
	
	AttackPosition += -ControlledPawn->GetActorForwardVector() * ControlledPawn->GetAttackRange();
	AttackPosition -= -ControlledPawn->GetActorForwardVector() * Offset;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector("AttackPosition", AttackPosition);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
