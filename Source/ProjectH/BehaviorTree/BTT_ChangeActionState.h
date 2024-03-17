#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChangeActionState.generated.h"

enum class EMonsterAction : uint8;

UCLASS()
class PROJECTH_API UBTT_ChangeActionState : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTT_ChangeActionState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		EMonsterAction Action;
};
