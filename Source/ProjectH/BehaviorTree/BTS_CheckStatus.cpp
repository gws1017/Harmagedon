#include "BehaviorTree/BTS_CheckStatus.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Character/Enemy.h"
#include "Global.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_CheckStatus::UBTS_CheckStatus()
{
	NodeName = TEXT("CheckStatus");
	Interval = 1.0f;
}

void UBTS_CheckStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemy* ControlledPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	CheckNull(ControlledPawn);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentHP", ControlledPawn->GetCurrentHP());
	OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetKey", ControlledPawn->GetTarget());

}