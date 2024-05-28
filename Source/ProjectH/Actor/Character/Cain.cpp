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
#include "Component/CainPatternInfo.h"

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

	RightHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandTrigger"));
	RightHandTrigger->SetSphereRadius(30.0f);
	RightHandTrigger->SetupAttachment(GetMesh(), "SwordRSocket");

	LeftHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandTrigger"));
	LeftHandTrigger->SetSphereRadius(30.0f);
	LeftHandTrigger->SetupAttachment(GetMesh(), "SwordLSocket");

	RightFootTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("RightFootTrigger"));
	//Trigger->SetCollisionProfileName(CPROFILE_PDTRIGGER);
	RightFootTrigger->SetSphereRadius(30.0f);
	RightFootTrigger->SetupAttachment(GetMesh(), "RightFootSocket");

	LeftFootTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFootTrigger"));
	//Trigger->SetCollisionProfileName(CPROFILE_PDTRIGGER);
	LeftFootTrigger->SetSphereRadius(30.0f);
	LeftFootTrigger->SetupAttachment(GetMesh(), "LeftFootSocket");

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

	TArray<const TCHAR*> montageRefs = {
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainBattlecry.AM_CainBattlecry'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainPunch1.AM_CainPunch1'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainHook1.AM_CainHook1'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainGrab.AM_CainGrab'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainThrowAway.AM_CainThrowAway'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainSmashHead.AM_CainSmashHead'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainThrowRock.AM_CainThrowRock'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainStrongKick.AM_CainStrongKick'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainDash.AM_CainDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainBackDash.AM_CainBackDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainLeftDash.AM_CainLeftDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainRightDash.AM_CainRightDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainPunch2.AM_CainPunch2'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainUppercut.AM_CainUppercut'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainHook2.AM_CainHook2'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainThrowDown.AM_CainThrowDown'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainStomp2.AM_CainStomp2'")
	};

	TArray<class UAnimMontage*> BTMontages;
	for (auto StrRef : montageRefs)
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(StrRef);
		if (MontageRef.Object)
		{
			BTMontages.Push(MontageRef.Object);
		}
	}

	TArray<TArray<float>> AttackDamages = {
		{221.0f},
		{284.0f},
		{268.0f},
		{282.0f},
		{184.0f, 321.0f},
		{244.0f},
		{-1.0f},
		{-1.0f},
		{-1.0f},
		{-1.0f},
		{188.0f},
		{244.0f},
		{212.0f},
		{22.0f, 284.0f},
		{120.0f, 340.0f}
	};
	AttackDamages = {
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
		{1.0f},
	};


	TArray<TArray<FString>> AttackParts = {
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
		{"Socket"},
	};


	int i = -1;
	for (auto const& StrRef : montageRefs)
	{
		++i;

		FName patternName = FName(*FString::Printf(TEXT("info%d"), i));
		UCainPatternInfo* info = CreateDefaultSubobject<UCainPatternInfo>(patternName);
		info->Init(AttackDamages[i], AttackParts[i], BTMontages[i]);
		PatternInfoes.Push(info);
	}
}
 
void ACain::BeginPlay()
{
	Super::BeginPlay();
}

void ACain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// ��������Ʈ�� �Լ� ���, �ٸ� ��������Ʈ���� �ڿ� ȣ���Ű��
	RightHandTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACain::OnOverlapBegin);
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
	CurrentStatus = static_cast<uint16>(InAnimMon);
	AnimInstance->Montage_Play(PatternInfoes[static_cast<uint16>(InAnimMon)]->BTMontage, 1.0f);

	// ��Ÿ�ְ� ������ �޺� ���� �Լ� ȣ�� ����
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::MontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, PatternInfoes[static_cast<uint16>(InAnimMon)]->BTMontage);
}

void ACain::AttackHitCheck()
{
	AttackCheckStart = true;
	AttackCount = 0;
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
	std::vector<float> CainAttackDamage(PatternInfoes.Num(), 20.0f);

	FDamageEvent DamageEvent;
	APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
	if (playerActor && AttackCheckStart)
	{
		playerActor->TakeDamage(PatternInfoes[CurrentStatus]->AttackDamages[AttackCount], DamageEvent, GetController(), this);
		AttackCheckStart = false;

		if (CurrentStatus == static_cast<uint16>(EMontages::STRONGKICK))
		{
			FVector LookVector = playerActor->GetActorLocation() - GetActorLocation();
			if (LookVector.X < 0.0f)
				LookVector.X *= -1.0f;
			if (LookVector.Y < 0.0f)
				LookVector.Y *= -1.0f;
			LookVector.Z = 0.0f;

			LookVector.Normalize();

			playerActor->SetActorLocation(playerActor->GetActorLocation() - LookVector * 200.0f);
		}
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