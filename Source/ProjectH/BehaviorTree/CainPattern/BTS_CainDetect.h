// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CainDetect.generated.h"

 /**************************************************************************************************
 * 감지 서비스
 * 거리 별로 타겟에 저장
 *
 * @author	조현식
 * @date	2024-05-04
 **************************************************************************************************/
UCLASS()
class PROJECTH_API UBTS_CainDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_CainDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	bool DetectTarget(TArray<FOverlapResult>& OverlapResults, class APawn* ControllingPawn, float DetectRadius);
	bool SetKey(const TArray<FOverlapResult>& OverlapResults, UBehaviorTreeComponent& OwnerComp, const FName& text, 
		bool DetectResult, class APawn* ControllingPawn, float DetectRadius);
};
