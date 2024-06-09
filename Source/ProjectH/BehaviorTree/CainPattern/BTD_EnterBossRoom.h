// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_EnterBossRoom.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTD_EnterBossRoom : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_EnterBossRoom();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
