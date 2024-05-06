// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_HealthIsHalf.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTD_HealthIsHalf : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_HealthIsHalf();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
