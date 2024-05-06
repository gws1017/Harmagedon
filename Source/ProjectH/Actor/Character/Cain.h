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

	EMoveState GetMoveState() const { return MoveState; }

protected:
	virtual void BeginPlay() override;

// AI 인터페이스
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
	// 배틀 크라이 몽타주 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> BattleCryMontage;

	// 스트레이트 몽타주 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> StraightMontage;

	// 이동 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	EMoveState MoveState;
};
