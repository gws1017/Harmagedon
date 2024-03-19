#include "BehaviorTree/BTS_UpdateRanged.h"
#include "Actor/Character/NormalMonster.h"
#include "Global.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_UpdateRanged::UBTS_UpdateRanged()
{
	NodeName = TEXT("UpdateRange");
	Interval = 1.0f;
}

void UBTS_UpdateRanged::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());

	CheckNull(ControlledPawn);
	float range = ControlledPawn->GetAttackRange();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsRanged", ControlledPawn->IsRanged(range));
}
