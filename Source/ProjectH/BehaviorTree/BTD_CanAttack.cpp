#include "BehaviorTree/BTD_CanAttack.h"
#include "Actor/Character/NormalMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CanAttack::UBTD_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTD_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	ANormalMonster* ControlledPawn = Cast<ANormalMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return false;
	
	result = ControlledPawn->CanAttack();
	return result;
}
