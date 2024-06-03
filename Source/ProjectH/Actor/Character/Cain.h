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

	FORCEINLINE class USphereComponent* GetTrigger() { return RightHandTrigger; }

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
	virtual void PlayMontageByAI(EPattern AnimMon) override;

	virtual float GetAIDetectRoomRange() override;
	virtual bool IsHealthUnderHalf() override;
	virtual float GetAITurnSpeed() override;
	virtual bool AllowNextPattern() override;

	FCainMontageFinished OnMontageFinished;



// 노티파이 인터페이스
protected:
	virtual void AttackHitCheck() override;
	virtual void AttackHitCheck2() override;
	virtual void AttackHitCheckEnd() override;

protected:
	void MontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

// UI 위젯
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget/*, Meta = (AllowPrivateAccess = "true")*/)
	TObjectPtr<class UWidgetComponent> HpBar;

protected:
	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<class USphereComponent> RightHandTrigger;

	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<class USphereComponent> LeftHandTrigger;

	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<class USphereComponent> RightFootTrigger;

	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<class USphereComponent> LeftFootTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	TArray<class UCainPatternInfo*> PatternInfoes;


	uint8 CurrentStatus;
	int32 AttackCount;
	int32 TakeDamageCount = 0;
	bool bAllowNextPattern = false;

	enum {RIGHTHAND = 0, LEFTHAND, RIGHTFOOT, LEFTFOOT, ROCK, SPLASH};

	TArray<FString> AttackMeans = {
		TEXT("RightHand"),
		TEXT("LeftHand"),
		TEXT("RightFoot"),
		TEXT("LeftFoot"),
		TEXT("Rock"),
		TEXT("Splash")
	};

	TMap<FString, FString> AttackSocketNames = {
		{TEXT("RightHand"),TEXT("SwordRSocket")},
		{TEXT("LeftHand"),TEXT("SwordLSocket")},
		{TEXT("RightFoot"),TEXT("RightFootSocket")},
		{TEXT("LeftFoot"),TEXT("LeftFootSocket")},
	//TEXT("Rock"),
	//TEXT("Splash")
	};



	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	bool AttackCheckStart = false;
};
