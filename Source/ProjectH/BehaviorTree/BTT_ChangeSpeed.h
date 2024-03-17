// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTT_ChangeSpeed : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTT_ChangeSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float Speed;
};
