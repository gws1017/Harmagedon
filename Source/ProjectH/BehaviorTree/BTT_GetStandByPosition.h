#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetStandByPosition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_GetStandByPosition : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTT_GetStandByPosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float Offset;
};
