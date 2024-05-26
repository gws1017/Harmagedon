// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/Cain.h"
#include "Components/SphereComponent.h"
#include "Actor/Controller/CainController.h"
#include "Components/CapsuleComponent.h"
#include "Data/HCollision.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "UI/BossHpBarWidget.h"
#include "UI/BossHUDWidget.h"
#include "UI/BossWidgetComponent.h"
#include "Actor/Controller/BasicPlayerController.h"

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

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	//Trigger->SetCollisionProfileName(CPROFILE_PDTRIGGER);
	Trigger->SetSphereRadius(30.0f);
	Trigger->SetupAttachment(GetMesh(), L"SwordRSocket");

	// ���� ������Ʈ ����
	HpBar = CreateDefaultSubobject<UBossWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());

	//HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HpBar->SetHiddenInGame(true);
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/Blueprint/WBP_BossHPBar.WBP_BossHPBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MaxHP = 2462.0;
	HP = 2462.0;
}
 
void ACain::BeginPlay()
{
	Super::BeginPlay();
}

void ACain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// ��������Ʈ�� �Լ� ���, �ٸ� ��������Ʈ���� �ڿ� ȣ���Ű��
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACain::OnOverlapBegin);
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
	CurrentStatus = static_cast<uint8>(InAnimMon);
	AnimInstance->Montage_Play(BTMontages[static_cast<uint8>(InAnimMon)], 1.0f);

	// ��Ÿ�ְ� ������ �޺� ���� �Լ� ȣ�� ����
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::MontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BTMontages[static_cast<uint8>(InAnimMon)]);
}

void ACain::AttackHitCheck()
{
	AttackCheckStart = true;
}

void ACain::MontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// �޺� �� ���·� �ʱ�ȭ
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnMontageFinished.ExecuteIfBound();
}

void ACain::SetupCharacterWidget(UBossHpBarWidget* InUserWidget)
{
	UBossHpBarWidget* HpBarWidget = Cast<UBossHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(MaxHP);
		HpBarWidget->UpdateHpBar(HP);

		// �ڽ��� Hp�� ���� UpdateHpBar�Լ��� hp ���� �˸��� ����
		OnHpChanged.AddUObject(HpBarWidget, &UBossHpBarWidget::UpdateHpBar);
	}
}

void ACain::SetupHUDWidget(UBossHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateHpBar(HP);

		// ��������Ʈ�� �Լ� ���
		OnHpChanged.AddUObject(InHUDWidget, &UBossHUDWidget::UpdateHpBar);
	}
}

void ACain::SetHp(float NewHp)
{
	HP = NewHp;

	OnHpChanged.Broadcast(HP);
}

void ACain::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	// ������ ����
	std::vector<float> CainAttackDamage(BTMontages.Num(), 20.0f);

	FDamageEvent DamageEvent;
	APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
	if (playerActor && AttackCheckStart)
	{
		playerActor->TakeDamage(CainAttackDamage[CurrentStatus], DamageEvent, GetController(), this);
		AttackCheckStart = false;
	}
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

float ACain::GetAITurnSpeed()
{
	return 2.0f;
}

//float ACain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//	// �ӽ÷� �ٷ� ���ó��
//	SetDead();
//	// ������ ������ ����
//	Stat->ApplyDamage(DamageAmount);
//
//	return DamageAmount;
//}
//
//float ACain::ApplyDamage(float InDamage)
//{
//	const float PrevHp = CurrentHp;
//	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
//
//	//hp ���� - hp ���� �� �˸�
//	SetHp(PrevHp - ActualDamage);
//	if (CurrentHp <= KINDA_SMALL_NUMBER)
//	{
//		// hp = 0 �˸��� ������ ��� ���� �˸���
//		OnHpZero.Broadcast();
//	}
//
//	return ActualDamage;
//}