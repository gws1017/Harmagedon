#include "BehaviorTree/BTT_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Actor/Character/Enemy.h"
#include "Global.h"

UBTT_TurnToTarget::UBTT_TurnToTarget()
{
	NodeName = ("TurnToTarget");
}

EBTNodeResult::Type UBTT_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemy* ControlledPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	CheckNullResult(ControlledPawn->GetTarget(), EBTNodeResult::Failed);

	FRotator rot = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(),
		ControlledPawn->GetCombatTargetLocation());
	rot.Pitch = 0.f;
	ControlledPawn->SetActorRotation(rot);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}