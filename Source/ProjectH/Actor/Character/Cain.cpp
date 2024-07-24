// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/Cain.h"
#include "Actor/Controller/CainController.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Component/CainPatternInfo.h"
#include "UI/BossHpBarWidget.h"
#include "UI/BossHUDWidget.h"
#include "UI/BossWidgetComponent.h"
#include "Data/HCollision.h"
#include "System/Sound/SoundManager.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

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
	RightHandTrigger->SetupAttachment(GetMesh(), TEXT("SwordRSocket"));

	LeftHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandTrigger"));
	LeftHandTrigger->SetSphereRadius(30.0f);
	LeftHandTrigger->SetupAttachment(GetMesh(), TEXT("SwordLSocket"));

	RightFootTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("RightFootTrigger"));
	RightFootTrigger->SetSphereRadius(30.0f);
	RightFootTrigger->SetupAttachment(GetMesh(), TEXT("RightFootSocket"));

	LeftFootTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFootTrigger"));
	LeftFootTrigger->SetSphereRadius(30.0f);
	LeftFootTrigger->SetupAttachment(GetMesh(), TEXT("LeftFootSocket"));

	MaxHP = 2462.0;
	HP = 2462.0;

	TArray<const TCHAR*> PatternRefs = {
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainStrongKick.AM_CainStrongKick'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainSmashHead.AM_CainSmashHead'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainThrowRock.AM_CainThrowRock'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainStomp1.AM_CainStomp1'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainHook1.AM_CainHook1'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainPunch1.AM_CainPunch1'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainGrab.AM_CainGrab'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainThrowAway.AM_CainThrowAway'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainBattlecry.AM_CainBattlecry'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainUppercut.AM_CainUppercut'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainPunch2.AM_CainPunch2'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainHook2.AM_CainHook2'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainThrowDown.AM_CainThrowDown'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainStomp2.AM_CainStomp2'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainDash.AM_CainDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainBackDash.AM_CainBackDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainLeftDash.AM_CainLeftDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainRightDash.AM_CainRightDash'"),
		TEXT("/Script/Engine.AnimMontage'/Game/Actor/Characters/Enemy/Boss/Animation/AM_CainGroggy.AM_CainGroggy'")
	};

	TArray<class UAnimMontage*> Patterns;
	for (auto StrRef : PatternRefs)
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(StrRef);
		if (MontageRef.Object)
		{
			Patterns.Push(MontageRef.Object);
		}
	}

	TArray<TArray<float>> AttackDamages = {
		{244.0f},
		{282.0f},
		{184.0f},
		{100.0f},
		{284.0f},
		{221.0f},
		{24.0f},
		{268.0f},
		{362.0f},
		{244.0f},
		{188.0f},
		{212.0f},
		{22.0f,284.0f},
		{120.0f},
		{},
		{},
		{},
		{},
		{}
	};

	TArray<TArray<FString>> AttackMeansByPattern = {
		{AttackMeans[RIGHTFOOT]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[RIGHTFOOT]},
		{AttackMeans[LEFTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[RIGHTHAND]},
		{},
		{AttackMeans[LEFTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[LEFTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[LEFTFOOT]},
		{},
		{},
		{},
		{},
		{}
	};

	uint8 punch1Number = static_cast<uint8>(EPattern::PUNCH1);
	uint8 grabNumber = static_cast<uint8>(EPattern::GRAB);
	uint8 punch2Number = static_cast<uint8>(EPattern::PUNCH2);

	for (uint8 i = 0; i< Patterns.Num(); ++i)
	{
		FName patternName = FName(*FString::Printf(TEXT("info%d"), i));
		UCainPatternInfo* info = CreateDefaultSubobject<UCainPatternInfo>(patternName);

		bool bAllowParrying = ((i == punch1Number) || (i == grabNumber) || (i == punch2Number)) ? true : false;

		info->Init(AttackDamages[i], AttackMeansByPattern[i], Patterns[i], bAllowParrying);
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

	OnHpZero.AddUObject(this, &ACain::SetDead);

	// ��������Ʈ�� �Լ� ���, �ٸ� ��������Ʈ���� �ڿ� ȣ���Ű��
	RightHandTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACain::OnOverlapBegin);
	LeftHandTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACain::OnOverlapBegin);
	RightFootTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACain::OnOverlapBegin);
	LeftFootTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACain::OnOverlapBegin);
}

void ACain::SetMontageFinDelegate(const FCainMontageFinished& InFinished)
{
	OnMontageFinished = InFinished;
}

void ACain::PlayMontageByAI(EPattern InAnimMon)
{
	// ������ �ӵ��� �޺� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	CurrentStatus = static_cast<uint8>(InAnimMon);
	AnimInstance->Montage_Play(PatternInfoes[CurrentStatus]->BTMontage, 1.0f);

	// ��Ÿ�ְ� ������ �޺� ���� �Լ� ȣ�� ����
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::MontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, PatternInfoes[static_cast<uint8>(InAnimMon)]->BTMontage);

	if (CurrentStatus == static_cast<uint8>(EPattern::HOOK1)
		|| CurrentStatus == static_cast<uint8>(EPattern::THROWAWAY)
		|| CurrentStatus == static_cast<uint8>(EPattern::STOMP2))
	{
		bAllowNextPattern = false;
	}
}

void ACain::AttackHitCheck()
{
	AttackCheckStart = true;
	AttackCountInPattern = 0;
}

void ACain::AttackHitCheckEnd()
{
	AttackCheckStart = false;
}

void ACain::MontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// �޺� �� ���·� �ʱ�ȭ
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnMontageFinished.ExecuteIfBound();
}

float ACain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	++HitCount;

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(DamageCauser->GetOwner());
	if(playerCharacter == nullptr)
		playerCharacter = Cast<APlayerCharacter>(DamageCauser);

	ABasicPlayerController* playerController = Cast<ABasicPlayerController>(playerCharacter->GetController());
	SetupHUDWidget(playerController->GetBossHUD());

	if (HP <= KINDA_SMALL_NUMBER)
	{
		// hp = 0 �˸��� ������ ��� ���� �˸���
		OnHpZero.Broadcast();
		playerController->HideBossHUD();
	}

	return ActualDamage;
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
	ACain* CainActor = Cast<ACain>(OtherActor);
	if (CainActor)
		return;

	TArray<FString> const& CurrentAttackMeans = PatternInfoes[CurrentStatus]->AttackMeans;
	if (CurrentAttackMeans.IsEmpty())
		return;

	FString const& CurrentAttackMean = PatternInfoes[CurrentStatus]->AttackMeans[AttackCountInPattern];
	auto const& CurrentAttackDamage = PatternInfoes[CurrentStatus]->AttackDamages[AttackCountInPattern];


	if (PatternInfoes[CurrentStatus]->AttackMeans.IsEmpty())
		return;

	if (CurrentAttackMean == AttackMeans[RIGHTHAND]
		&& OverlappedComponent != RightHandTrigger)
	{
		return;
	}
	else if (CurrentAttackMean == AttackMeans[LEFTHAND]
		&& OverlappedComponent != LeftHandTrigger)
	{
		return;
	}
	else if (CurrentAttackMean == AttackMeans[RIGHTFOOT]
		&& OverlappedComponent != RightFootTrigger)
	{
		return;
	}
	else if (CurrentAttackMean == AttackMeans[LEFTFOOT]
		&& OverlappedComponent != LeftFootTrigger)
	{
		return;
	}
	//else if (CurrentAttackMean == AttackMeans[SPLASH]
	//	&& OverlappedComponent != SplashTrigger)
	//{
	//	return;
	//}

	std::vector<float> CainAttackDamage(PatternInfoes.Num(), 20.0f);

	FDamageEvent DamageEvent;
	APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
	if (!playerActor)
		return;

	// �÷��̾� ĳ���� ����
	if (playerActor->GetCapsuleComponent() == OtherComp && AttackCheckStart)
	{
		// ������ ����
		playerActor->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
		playerActor->Hit(OverlappedComponent->GetComponentLocation());
		AttackCheckStart = false;

		// �Ʒ��� ������ ���� �� ó��
		if (CurrentStatus == static_cast<uint8>(EPattern::STRONGKICK))
		{
			playerActor->LaunchCharacter(GetActorForwardVector() * 1000, false, false);
		}

		if (CurrentStatus == static_cast<uint8>(EPattern::GRAB))
		{
			playerActor->GetCharacterMovement()->DisableMovement();
			playerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			playerActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(*AttackSocketNames[CurrentAttackMean]));
			HoldingPlayer = playerActor;
		}


		if (CurrentStatus == static_cast<uint8>(EPattern::SMASHHEAD)
			|| CurrentStatus == static_cast<uint8>(EPattern::STOMP1)
			|| CurrentStatus == static_cast<uint8>(EPattern::STOMP2))
		{
			// BTD�� ���� �ǰ� 3������ ����
			// �ǰ� ī��Ʈ�� 0���� �ʱ�ȭ
			HitCount = 0;
		}

		if (CurrentStatus == static_cast<uint8>(EPattern::PUNCH1)
			|| CurrentStatus == static_cast<uint8>(EPattern::GRAB)
			|| CurrentStatus == static_cast<uint8>(EPattern::PUNCH2))
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && AnimInstance->Montage_IsPlaying(PatternInfoes[CurrentStatus]->BTMontage))
			{
				AnimInstance->Montage_Stop(0.2f, PatternInfoes[CurrentStatus]->BTMontage);
			}

			bAllowNextPattern = true;
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

bool ACain::AllowNextPattern()
{
	return bAllowNextPattern;
}

bool ACain::IsFirstPhase()
{
	return bFirstPhase;
}

int32 ACain::GetStatus()
{
	return CurrentStatus;
}

void ACain::ChangeIntoSecondPhase()
{
	bFirstPhase = false;
}

int32 ACain::GetHitCount()
{
	return HitCount;
}

void ACain::SetPrevRandomNumber(int32 number)
{
	PrevRandomNumber = number;
}

int32 ACain::GetPrevRandomNumber()
{
	return PrevRandomNumber;
}

void ACain::SetDead()
{
	// �̵� �Ұ� ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();

	// �浹���� ����
	SetActorEnableCollision(false);

	// hp�� ����� (�����Ϳ��� üũ)

	// AI��Ʈ�ѷ���� �����ϱ�
	ACainController* PdAIController = Cast<ACainController>(GetController());
	if (PdAIController)
	{
		PdAIController->StopAI();
	}

	// DeadEventDelayTime�� ���� �� �ı�
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);

	ASoundManager::GetSoundManager()->PlaySFX2D(ESFXType::ESFXType_Restore);
	ASoundManager::GetSoundManager()->SetBGM(EBGMType::EBGMType_InGame);
	ASoundManager::GetSoundManager()->PlayBGM();
}

void ACain::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// ���� ��Ÿ�� ����
	AnimInstance->StopAllMontages(0.0f);
	// ��� ��Ÿ�� ���
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}