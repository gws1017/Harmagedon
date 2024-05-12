// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "Interface/CainPatternInterface.h"
#include "Cain.generated.h"

 /**************************************************************************************************
 * ���� ���� (ī��)
 *
 * @author	������
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

// AI �������̽�
protected:
	virtual void SetMontageFinDelegate(const FCainMontageFinished& InOnAttackFinished) override;
	virtual void PlayMontageByAI(EMontages AnimMon) override;

	virtual float GetAIDetectRoomRange() override;
	virtual bool IsHealthUnderHalf() override;

	FCainMontageFinished OnMontageFinished;

protected:
	void MontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);



protected:
	// ��Ÿ�� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> BTMontages;
};
