#include "BehaviorTree/CainPattern/BTD_EnterBossRoom.h"
//#include "Gimmick/BossGimmick.h"
#include "Actor/Objects/SceneTrigger.h"
#include "Global.h"
UBTD_EnterBossRoom::UBTD_EnterBossRoom()
{
}

bool UBTD_EnterBossRoom::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	TArray<ASceneTrigger*> BossTrigger;
	UHelpers::FindActors<ASceneTrigger>(GetWorld(), BossTrigger);
	if (BossTrigger.IsEmpty())
	{
		check(false);
		return false;
	}

	return BossTrigger[0]->IsPlayerEnterRoom();
}
