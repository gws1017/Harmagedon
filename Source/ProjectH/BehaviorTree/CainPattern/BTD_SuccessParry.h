// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_SuccessParry.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTD_SuccessParry : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_SuccessParry();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
