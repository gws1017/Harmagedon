#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateRanged.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTS_UpdateRanged : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTS_UpdateRanged();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};