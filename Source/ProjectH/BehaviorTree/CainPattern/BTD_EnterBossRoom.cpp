#include "BehaviorTree/CainPattern/BTD_EnterBossRoom.h"
#include "Gimmick/BossGimmick.h"
#include "Utilities/UHelpers.h"

UBTD_EnterBossRoom::UBTD_EnterBossRoom()
{
}

bool UBTD_EnterBossRoom::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	TArray<class ABossGimmick*> bossGimmick;
	UHelpers::FindActors(GetWorld(), bossGimmick);
	if (bossGimmick.IsEmpty())
	{
		check(false);
		return false;
	}

	return bossGimmick[0]->IsPlayerEnterRoom();
}
