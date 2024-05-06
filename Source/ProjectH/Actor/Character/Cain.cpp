// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/Cain.h"
#include "Actor/Controller/CainController.h"
#include "Components/CapsuleComponent.h"
#include "Data/HCollision.h"
#include "GameFramework/CharacterMovementComponent.h"

ACain::ACain()
{
	// 배치되거나 스폰된 NPC 둘 다 AI로 통제받음
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 무브먼트 컴포넌트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_HCAPSULE);
	//GetMesh()->SetCollisionProfileName(CPROFILE_HPHSICSACTOR);
}

void ACain::BeginPlay()
{
	Super::BeginPlay();
}

void ACain::SetAIBattleCryDelegate(const FBattleCryFinished& InOnBattleCryFinished)
{
	OnBattleCryFinished = InOnBattleCryFinished;
}
void ACain::BattleCryByAI()
{
	// 이동 불가 설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 지정한 속도로 콤보 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(BattleCryMontage, 1.0f);

	// 몽타주가 끝나면 콤보 종료 함수 호출 예약
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::BattleCryEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BattleCryMontage);
}
void ACain::BattleCryEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 콤보 전 상태로 초기화
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	OnBattleCryFinished.ExecuteIfBound();
}


void ACain::SetAIStraightDelegate(const FBattleCryFinished& InOnBattleCryFinished)
{
	OnBattleCryFinished = InOnBattleCryFinished;
}
void ACain::StraightByAI()
{
	// 이동 불가 설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 지정한 속도로 콤보 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(StraightMontage, 1.0f);

	// 몽타주가 끝나면 콤보 종료 함수 호출 예약
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::StraightEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, StraightMontage);
}
void ACain::StraightEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 콤보 전 상태로 초기화
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	OnStraightFinished.ExecuteIfBound();
}



float ACain::GetAIDetectRoomRange()
{
	return 2000.0f;
}


float ACain::GetAIFarDetectRange()
{
	return 1000.0f;
}

float ACain::GetAINearDetectRange()
{
	return 500.0f;
}

bool ACain::IsRanged(float radius)
{
	return AEnemy::IsRanged(radius);
}

bool ACain::IsHealthUnderHalf()
{
	float healthRatio = GetCurrentHP() / GetMaxHP();

	return healthRatio <= 0.5;
}

void ACain::SetDashProperty()
{
	MoveState = EMoveState::Dash;
}