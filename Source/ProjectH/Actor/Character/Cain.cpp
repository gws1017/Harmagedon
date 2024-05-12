// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/Cain.h"
#include "Actor/Controller/CainController.h"
#include "Components/CapsuleComponent.h"
#include "Data/HCollision.h"
#include "GameFramework/CharacterMovementComponent.h"

ACain::ACain()
{
	// ��ġ�ǰų� ������ NPC �� �� AI�� ��������
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// �����Ʈ ������Ʈ ����
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
	// �̵� �Ұ� ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// ������ �ӵ��� �޺� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(BTMontages[static_cast<uint8>(InAnimMon)], 1.0f);

	// ��Ÿ�ְ� ������ �޺� ���� �Լ� ȣ�� ����
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::MontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BTMontages[static_cast<uint8>(InAnimMon)]);
}
void ACain::MontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// �޺� �� ���·� �ʱ�ȭ
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