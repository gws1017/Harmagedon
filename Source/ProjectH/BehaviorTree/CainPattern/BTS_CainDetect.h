// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CainDetect.generated.h"

 /**************************************************************************************************
 * ���� ����
 * �Ÿ� ���� Ÿ�ٿ� ����
 *
 * @author	������
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
};
