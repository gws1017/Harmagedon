// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainBattleCry.generated.h"

 /**************************************************************************************************
 * �½�ũ (��Ʋ ũ���� ����)
 *
 * @author	������
 * @date	2024-05-04
 **************************************************************************************************/
UCLASS()
class PROJECTH_API UBTT_CainBattleCry : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_CainBattleCry();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
