// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "Interface/CainPatternInterface.h"
#include "Interface/CainAnimNotifyInterface.h"
#include "Cain.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

class USphereComponent;
class UCainPatternInfo;
class USphereComponent;
class UBossHpBarWidget;
class UBossHUDWidget;
class APlayerCharacter;
class UAnimMontage;
class UWidgetComponent;
class UAnimMontage;


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

	FORCEINLINE USphereComponent* GetTrigger() { return RightHandTrigger; }

	void SetupCharacterWidget(UBossHpBarWidget* InUserWidget);
	void SetupHUDWidget(UBossHUDWidget* InHUDWidget);
	void SetHp(float NewHp);
	bool GetAttackCheck() const { return AttackCheckStart; };

	FOnHpChangedDelegate OnHpChanged;
	TObjectPtr<APlayerCharacter> HoldingPlayer;
	bool bAllowNextPattern = false;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

// AI 인터페이스
protected:
	virtual void SetMontageFinDelegate(const FCainMontageFinished& InOnAttackFinished) override;
	virtual void PlayMontageByAI(EPattern AnimMon) override;
	virtual void JumpMontageSection(FName SectionName, EPattern AnimMon) override;
	virtual void GroggyAnim() override;

	virtual float GetAIDetectRoomRange() override;
	virtual bool IsHealthUnderHalf() override;
	virtual float GetAITurnSpeed() override;
	virtual bool AllowNextPattern() override;
	virtual bool IsFirstPhase() override;
	virtual int32 GetStatus() override;
	virtual void ChangeIntoSecondPhase() override;
	virtual int32 GetHitCount() override;
	virtual void SetPrevRandomNumber(int32 number) override;
	virtual int32 GetPrevRandomNumber() override;
	virtual bool IsSuccessParry() override;
public:
	virtual void IsSuccessParry(bool flag) override { bSuccessParry = flag; }


	FCainMontageFinished OnMontageFinished;



// 노티파이 인터페이스
public:
	virtual void AttackHitCheck() override;
	virtual void AttackHitCheckEnd() override;
	virtual void StopAnim() override;

protected:
	void MontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// 사망
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	FOnHpZeroDelegate OnHpZero;
	float DeadEventDelayTime = 2.8f;

protected:
	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<USphereComponent> RightHandTrigger;

	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<USphereComponent> LeftHandTrigger;

	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<USphereComponent> RightFootTrigger;

	UPROPERTY(VisibleAnywhere, Category = trigger)
	TObjectPtr<USphereComponent> LeftFootTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	TArray<UCainPatternInfo*> PatternInfoes;

	uint8 CurrentStatus;
	int32 AttackCountInPattern;
	int32 HitCount = 0;

	enum {RIGHTHAND = 0, LEFTHAND, RIGHTFOOT, LEFTFOOT, ROCK, SPLASH};

	TArray<FString> AttackMeans = {
		TEXT("RightHand"),
		TEXT("LeftHand"),
		TEXT("RightFoot"),
		TEXT("LeftFoot"),
	};

	TMap<FString, FString> AttackSocketNames = {
		{TEXT("RightHand"),TEXT("SwordRSocket")},
		{TEXT("LeftHand"),TEXT("SwordLSocket")},
		{TEXT("RightFoot"),TEXT("RightFootSocket")},
		{TEXT("LeftFoot"),TEXT("LeftFootSocket")},
	};



	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	bool AttackCheckStart = false;

	bool bFirstPhase = true;
	int32 PrevRandomNumber;

	bool bSuccessParry = false;
};
