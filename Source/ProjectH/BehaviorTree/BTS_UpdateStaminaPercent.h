#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateStaminaPercent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTS_UpdateStaminaPercent : public UBTService
{
	GENERATED_BODY()
public:

	UBTS_UpdateStaminaPercent();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
