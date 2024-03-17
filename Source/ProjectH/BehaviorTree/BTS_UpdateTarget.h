#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTS_UpdateTarget : public UBTService
{
	GENERATED_BODY()
public:

	UBTS_UpdateTarget();
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
