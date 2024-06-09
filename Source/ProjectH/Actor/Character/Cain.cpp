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

	RightHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandTrigger"));
	RightHandTrigger->SetSphereRadius(20.0f);
	RightHandTrigger->SetupAttachment(GetMesh(), TEXT("SwordRSocket"));

	LeftHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandTrigger"));
	LeftHandTrigger->SetSphereRadius(20.0f);
	LeftHandTrigger->SetupAttachment(GetMesh(), TEXT("SwordLSocket"));

	RightFootTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("RightFootTrigger"));
	RightFootTrigger->SetSphereRadius(20.0f);
	RightFootTrigger->SetupAttachment(GetMesh(), TEXT("RightFootSocket"));

	LeftFootTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFootTrigger"));
	LeftFootTrigger->SetSphereRadius(20.0f);
	LeftFootTrigger->SetupAttachment(GetMesh(), TEXT("LeftFootSocket"));



	// 위젯 컴포넌트 생성
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
		{184.0f, 321.0f},
		{100.0f, 280.0f},
		{284.0f},
		{221.0f},
		{24.0f},
		{268.0f},
		{362.0f},
		{244.0f},
		{188.0f},
		{212.0f},
		{22.0f,284.0f},
		{120.0f, 340.0f},
		{},
		{},
		{},
		{},
		{}
	};

	TArray<TArray<FString>> AttackMeansByPattern = {
		{AttackMeans[RIGHTFOOT]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[RIGHTHAND], AttackMeans[ROCK]},
		{AttackMeans[RIGHTFOOT], AttackMeans[SPLASH]},
		{AttackMeans[LEFTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[SPLASH]},
		{AttackMeans[LEFTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[LEFTHAND]},
		{AttackMeans[RIGHTHAND]},
		{AttackMeans[LEFTFOOT], AttackMeans[SPLASH]},
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
	// 델리게이트에 함수 등록, 다른 델리게이트보다 뒤에 호출시키기
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
	// 이동 불가 설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 지정한 속도로 콤보 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	CurrentStatus = static_cast<uint8>(InAnimMon);
	AnimInstance->Montage_Play(PatternInfoes[CurrentStatus]->BTMontage, 1.0f);

	// 몽타주가 끝나면 콤보 종료 함수 호출 예약
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACain::MontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, PatternInfoes[static_cast<uint8>(InAnimMon)]->BTMontage);
}

void ACain::AttackHitCheck()
{
	AttackCheckStart = true;
	AttackCountInPattern = 0;
}

void ACain::AttackHitCheck2()
{
	AttackCheckStart = true;
	AttackCountInPattern = 1;
}

void ACain::AttackHitCheckEnd()
{
	AttackCheckStart = false;
}

void ACain::MontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 콤보 전 상태로 초기화
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnMontageFinished.ExecuteIfBound();
}

float ACain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	++HitCount;

	return Damage;
}

void ACain::SetupCharacterWidget(UBossHpBarWidget* InUserWidget)
{
	UBossHpBarWidget* HpBarWidget = Cast<UBossHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(MaxHP);
		HpBarWidget->UpdateHpBar(HP);

		// 자신의 Hp바 내부 UpdateHpBar함수가 hp 변경 알림에 구독
		OnHpChanged.AddUObject(HpBarWidget, &UBossHpBarWidget::UpdateHpBar);
	}
}

void ACain::SetupHUDWidget(UBossHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateHpBar(HP);

		// 델리게이트에 함수 등록
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

	if (playerActor && CurrentStatus == static_cast<uint8>(EPattern::THROWAWAY))
	{
		playerActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		playerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		playerActor->GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_HCAPSULE);
		playerActor->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		playerActor->SetActorRotation(FRotator::ZeroRotator);
		playerActor->LaunchCharacter(GetActorForwardVector() * 2000, true, true);

		bAllowNextPattern = false;
		//땅에 부딪쳤을 때 데미지
	}


	// 플레이어 캐릭터 감지
	if (playerActor->GetCapsuleComponent() == OtherComp && AttackCheckStart)
	{
		// 데미지 전달
		playerActor->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
		AttackCheckStart = false;

		// 아래는 데미지 입은 후 처리
		if (CurrentStatus == static_cast<uint8>(EPattern::STRONGKICK))
		{
			playerActor->LaunchCharacter(GetActorForwardVector() * 1000, false, false);
		}

		if (CurrentStatus == static_cast<uint8>(EPattern::GRAB))
		{
			playerActor->GetCharacterMovement()->DisableMovement();
			playerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			playerActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(*AttackSocketNames[CurrentAttackMean]));
			//prevRotator = playerActor->GetActorRotation();
		}


		if (CurrentStatus == static_cast<uint8>(EPattern::SMASHHEAD)
			|| CurrentStatus == static_cast<uint8>(EPattern::STOMP1)
			|| CurrentStatus == static_cast<uint8>(EPattern::STOMP2))
		{
			// BTD를 통해 피격 3번인지 판정
			// 피격 카운트를 0으로 초기화
			HitCount = 0;
		}

		if (CurrentStatus == static_cast<uint8>(EPattern::PUNCH1)
			|| CurrentStatus == static_cast<uint8>(EPattern::GRAB)
			|| CurrentStatus == static_cast<uint8>(EPattern::THROWDOWN))
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && AnimInstance->Montage_IsPlaying(PatternInfoes[CurrentStatus]->BTMontage))
			{
				AnimInstance->Montage_Stop(0.2f, PatternInfoes[CurrentStatus]->BTMontage);
			}

			bAllowNextPattern = true;
			AttackCheckStart = true;
		}
		else if (CurrentStatus == static_cast<uint8>(EPattern::HOOK1)
			|| CurrentStatus == static_cast<uint8>(EPattern::THROWAWAY)
			|| CurrentStatus == static_cast<uint8>(EPattern::STOMP2))
		{
			bAllowNextPattern = false;
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
