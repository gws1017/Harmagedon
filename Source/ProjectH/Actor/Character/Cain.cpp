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
}

void ACain::BeginPlay()
{
	Super::BeginPlay();
}

void ACain::SetMontageFinDelegate(const FCainMontageFinished& InFinished)
{
	OnMontageFinished = InFinished;
}

void ACain::PlayMontageByAI(EMontages InAnimMon)
{
	// 이동 불가 설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 지정한 속도로 콤보 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(BTMontages[static_cast<uint8>(InAnimMon)], 1.0f);

	// 몽타주가 끝나면 콤보 종료 함수 호출 예약
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::MontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BTMontages[static_cast<uint8>(InAnimMon)]);
}
void ACain::MontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 콤보 전 상태로 초기화
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnMontageFinished.ExecuteIfBound();
}

float ACain::GetAIDetectRoomRange()
{
	return 2000.0f;
}

bool ACain::IsHealthUnderHalf()
{
	float healthRatio = GetCurrentHP() / GetMaxHP();
	return healthRatio <= 0.5;
}