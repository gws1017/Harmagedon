#include "BehaviorTree/BTS_UpdateStaminaPercent.h"
#include "Actor/Character/NormalMonster.h"
#include "Global.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_UpdateStaminaPercent::UBTS_UpdateStaminaPercent()
{
	NodeName = TEXT("UpdateStaminaPercent");
	Interval = 1.0f;
}

void UBTS_UpdateStaminaPercent::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());

	CheckNull(ControlledPawn);
	float StaminaPercent = ControlledPawn->GetStamina() / ControlledPawn->GetMaxStamina() * 100.f;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("StaminaPercent", StaminaPercent);
}
