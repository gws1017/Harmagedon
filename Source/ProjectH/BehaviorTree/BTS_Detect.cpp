#include "BehaviorTree/BTS_Detect.h"
#include "Actor/Character/Enemy.h"
#include "Global.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemy* ControlledPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	float radius = ControlledPawn->GetAttackRange();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsRanged", ControlledPawn->IsRanged(radius));

}