#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetPatrolPosition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_GetPatrolPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTT_GetPatrolPosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
