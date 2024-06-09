// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_DrawRandomNumber.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTS_DrawRandomNumber : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_DrawRandomNumber();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	int32 RandomNumber;
};
