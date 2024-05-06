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

	EMoveState GetMoveState() const { return MoveState; }

protected:
	virtual void BeginPlay() override;

// AI �������̽�
protected:
	virtual void SetAIBattleCryDelegate(const FBattleCryFinished& InOnAttackFinished) override;
	FBattleCryFinished OnBattleCryFinished;
	virtual void BattleCryByAI() override;

	virtual void SetAIStraightDelegate(const FBattleCryFinished& InOnAttackFinished) override;
	FBattleCryFinished OnStraightFinished;
	virtual void StraightByAI() override;


	virtual float GetAIDetectRoomRange() override;
	virtual float GetAIFarDetectRange() override;
	virtual float GetAINearDetectRange() override;
	virtual bool IsRanged(float radius) override;
	virtual bool IsHealthUnderHalf() override;
	virtual void SetDashProperty() override;

protected:
	void BattleCryEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void StraightEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

protected:
	// ��Ʋ ũ���� ��Ÿ�� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> BattleCryMontage;

	// ��Ʈ����Ʈ ��Ÿ�� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> StraightMontage;

	// �̵� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	EMoveState MoveState;
};
