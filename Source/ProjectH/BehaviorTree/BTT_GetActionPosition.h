// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetActionPosition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_GetActionPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTT_GetActionPosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float Offset;
};