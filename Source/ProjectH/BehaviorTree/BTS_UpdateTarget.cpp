#include "BehaviorTree/BTS_UpdateTarget.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Character/Enemy.h"
#include "Global.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_UpdateTarget::UBTS_UpdateTarget()
{
	NodeName = TEXT("UpdateTarget");
	Interval = 1.0f;
}

void UBTS_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemy* ControlledPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	CheckNull(ControlledPawn);

	OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetKey", ControlledPawn->GetTarget());
}
