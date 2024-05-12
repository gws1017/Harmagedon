// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "Interface/CainPatternInterface.h"
#include "Cain.generated.h"

 /**************************************************************************************************
 * 보스 몬스터 (카인)
 *
 * @author	조현식
 * @date	2024-05-03
 **************************************************************************************************/
UCLASS()
class PROJECTH_API ACain : public AEnemy, public ICainPatternInterface
{
	GENERATED_BODY()
	
public:
	ACain();

protected:
	virtual void BeginPlay() override;

// AI 인터페이스
protected:
	virtual void SetMontageFinDelegate(const FCainMontageFinished& InOnAttackFinished) override;
	virtual void PlayMontageByAI(EMontages AnimMon) override;

	virtual float GetAIDetectRoomRange() override;
	virtual bool IsHealthUnderHalf() override;

	FCainMontageFinished OnMontageFinished;

protected:
	void MontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);



protected:
	// 몽타주 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> BTMontages;
};
