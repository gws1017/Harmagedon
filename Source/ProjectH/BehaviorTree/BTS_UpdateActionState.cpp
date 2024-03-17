#include "BehaviorTree/BTS_UpdateActionState.h"
#include "Actor/Character/NormalMonster.h"
#include "Global.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_UpdateActionState::UBTS_UpdateActionState()
{
	NodeName = TEXT("UpdateActionState");
	Interval = 1.0f;
}

void UBTS_UpdateActionState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());

	CheckNull(ControlledPawn);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum("ActionState", StaticCast<uint8>(ControlledPawn->GetActionState()));
}
