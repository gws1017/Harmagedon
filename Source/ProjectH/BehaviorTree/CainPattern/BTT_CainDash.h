// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CainDash.generated.h"

 /**************************************************************************************************
 * ��� �½�ũ
 *
 * @author	������
 * @date	2024-05-06
 **************************************************************************************************/
UCLASS()
class PROJECTH_API UBTT_CainDash : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_CainDash();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
