// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "Interface/CainPatternInterface.h"
#include "Interface/CainAnimNotifyInterface.h"
#include "Cain.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);



 /**************************************************************************************************
 * 보스 몬스터 (카인)
 *
 * @author	조현식
 * @date	2024-05-03
 **************************************************************************************************/
UCLASS()
class PROJECTH_API ACain 
	: public AEnemy
	, public ICainPatternInterface
	, public ICainAnimNotifyInterface
{
	GENERATED_BODY()
	
public:
	ACain();

	FORCEINLINE class USphereComponent* GetTrigger() { return Trigger; }

	void SetupCharacterWidget(class UBossHpBarWidget* InUserWidget);
	void SetupHUDWidget(class UBossHUDWidget* InHUDWidget);
	void SetHp(float NewHp);

	FOnHpChangedDelegate OnHpChanged;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

// AI 인터페이스
protected:
	virtual void SetMontageFinDelegate(const FCainMontageFinished& InOnAttackFinished) override;
	virtual void PlayMontageByAI(EMontages AnimMon) override;

	virtual float GetAIDetectRoomRange() override;
	virtual bool IsHealthUnderHalf() override;
	virtual float GetAITurnSpeed() override;

	FCainMontageFinished OnMontageFinished;



// 노티파이 인터페이스
protected:
	virtual void AttackHitCheck();

protected:
	void MontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

// UI 위젯
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget/*, Meta = (AllowPrivateAccess = "true")*/)
	TObjectPtr<class UWidgetComponent> HpBar;

protected:
	UPROPERTY(VisibleAnywhere, Category = sphere)
	TObjectPtr<class USphereComponent> Trigger;

	// 몽타주 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> BTMontages;

	uint32 CurrentStatus;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	bool AttackCheckStart = false;

	//float MaxHp = 2462.0;
	//float CurrentHp = 2462.0;
};
