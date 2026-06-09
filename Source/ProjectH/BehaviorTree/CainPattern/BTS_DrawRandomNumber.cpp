// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTS_DrawRandomNumber.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h" // FMath 헤더 포함
#include "Data/CainAIKey.h"
#include "Component/CainAIDirector.h"
#include "Actor/Character/Cain.h"


UBTS_DrawRandomNumber::UBTS_DrawRandomNumber()
{
	NodeName = TEXT("DrawRandomNumber");
	Interval = 1.0f;
}

void UBTS_DrawRandomNumber::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 비헤이비어 트리 소유한 폰 갖고오기
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return;
	}

	// 폰 위치와 폰이 속한 월드 가져오기
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (!World)
	{
		return;
	}

	// 폰을 AI폰으로 변환
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return;
	}

	if (!AIPawn->AllowNextPattern())
	{
		return;
	}

	ACain* Cain = Cast<ACain>(ControllingPawn);
	UCainAIDirector* Director = Cain
		? Cain->FindComponentByClass<UCainAIDirector>()
		: nullptr;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	const int32 CurrentBB = BB->GetValueAsInt(BBKEY_RANDOMNUMBER);
	const bool bStaleSameNumber = (AIPawn->GetPrevRandomNumber() == CurrentBB);

	const int32 Mode = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CainPatternMode"))
		? IConsoleManager::Get().FindConsoleVariable(TEXT("r.CainPatternMode"))->GetInt()
		: 0;

	auto CommitRandomNumber = [&](int32 NewNumber)
	{
		RandomNumber = NewNumber;
		AIPawn->SetPrevRandomNumber(RandomNumber);
		BB->SetValueAsInt(BBKEY_RANDOMNUMBER, RandomNumber);
	};

	auto FallbackRandom = [&](const TCHAR* Reason)
	{
		int32 Fallback = FMath::RandRange(1, 100);
		if (Fallback == AIPawn->GetPrevRandomNumber())
		{
			Fallback = (Fallback % 100) + 1;
		}
		CommitRandomNumber(Fallback);
		UE_LOG(LogTemp, Log, TEXT("[CainAI][BTS] %s  Fallback RandomNumber=%d"), Reason, RandomNumber);
		if (Director)
		{
			Director->UpdateDebugWidget(TEXT("[AI] FALLBACK"), Reason, FLinearColor(1.f, 0.5f, 0.f));
		}
	};

	// Mode 1 대기: BB·Prev를 idle sentinel으로 — 3(backdash) 등이 <55 공격 분기에 걸리는 것 방지
	auto HoldAttackUntilAI = [&]()
	{
		AIPawn->SetPrevRandomNumber(BB_IDLE_RANDOMNUMBER);
		BB->SetValueAsInt(BBKEY_RANDOMNUMBER, BB_IDLE_RANDOMNUMBER);
	};

	//기존 난수방식
	if (Mode == 0)
	{
		CommitRandomNumber(FMath::RandRange(1, 100));
		UE_LOG(LogTemp, Log, TEXT("[CainAI][BTS] Mode=0(Random)  RandomNumber=%d"), RandomNumber);
		if (Director) Director->UpdateDebugWidget(TEXT("[MODE 0] RANDOM"), TEXT(""), FLinearColor::Gray);
	}
	// Mode 1: AI 전용 — LLM 응답 전까지 BB 유지(대기), 난수 폴백 없음
	else if (Mode == 1)
	{
		if (!Director)
		{
			UE_LOG(LogTemp, Warning, TEXT("[CainAI][BTS] Mode=1  No Director — waiting"));
			HoldAttackUntilAI();
			return;
		}

		//덮어쓰기방지
		if (Director->IsFreshAIPatternInBB())
		{
			return;
		}

		const int32 AINum = Director->ConsumeCachedPatternNumber();
		if (AINum >= 1)
		{
			CommitRandomNumber(AINum);
			Director->SetFreshAIPattern(true);
			UE_LOG(LogTemp, Log, TEXT("[CainAI][BTS] Mode=1(AI)  RandomNumber=%d"), RandomNumber);
			return;
		}

		//요청 받아오는 중
		if (Director->IsRequestInFlight())
		{
			UE_LOG(LogTemp, Log, TEXT("[CainAI][BTS] Mode=1  WAITING for LLM..."));
			Director->UpdateDebugWidget(TEXT("[AI] THINKING..."), TEXT(""), FLinearColor::Yellow);
			HoldAttackUntilAI();
			return;
		}

		//요청 쿨타임
		if (Director->IsRateLimited())
		{
			UE_LOG(LogTemp, Log, TEXT("[CainAI][BTS] Mode=1  Rate limited, waiting..."));
			Director->UpdateDebugWidget(TEXT("[AI] COOLING DOWN..."), TEXT(""), FLinearColor(1.f, 0.5f, 0.f));
			HoldAttackUntilAI();
			return;
		}

		//비동기 요청
		Director->RequestNextPatternAsync();
		if (Director->IsRequestInFlight())
		{
			UE_LOG(LogTemp, Log, TEXT("[CainAI][BTS] Mode=1  Fired request, waiting..."));
			Director->UpdateDebugWidget(TEXT("[AI] THINKING..."), TEXT(""), FLinearColor::Yellow);
			HoldAttackUntilAI();
			return;
		}

		//잘못된 패턴
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][BTS] Mode=1  LLM unavailable — waiting (no fallback)"));
		Director->UpdateDebugWidget(TEXT("[AI] UNAVAILABLE"), TEXT("waiting"), FLinearColor::Red);
		HoldAttackUntilAI();
	}
	// Mode 2: AI 우선, 실패/대기 시 RandRang, 기존방식사용
	else
	{
		if (!Director)
		{
			FallbackRandom(TEXT("No Director"));
			return;
		}

		if (Director->IsFreshAIPatternInBB())
		{
			return;
		}

		const int32 AINum = Director->ConsumeCachedPatternNumber();
		if (AINum >= 1)
		{
			CommitRandomNumber(AINum);
			UE_LOG(LogTemp, Log, TEXT("[CainAI][BTS] Mode=2(AI)  RandomNumber=%d"), RandomNumber);
			return;
		}

		if (Director->IsRequestInFlight())
		{
			if (bStaleSameNumber)
			{
				FallbackRandom(TEXT("LLM wait"));
			}
			else
			{
				Director->UpdateDebugWidget(TEXT("[AI] THINKING..."), TEXT("Mode2"), FLinearColor::Yellow);
			}
			return;
		}

		if (Director->IsRateLimited())
		{
			if (bStaleSameNumber)
			{
				FallbackRandom(TEXT("Rate limit"));
			}
			else
			{
				Director->UpdateDebugWidget(TEXT("[AI] COOLING DOWN..."), TEXT("Mode2"), FLinearColor(1.f, 0.5f, 0.f));
			}
			return;
		}

		Director->RequestNextPatternAsync();
		if (Director->IsRequestInFlight())
		{
			if (bStaleSameNumber)
			{
				FallbackRandom(TEXT("LLM request started"));
			}
			return;
		}

		FallbackRandom(TEXT("LLM unavailable"));
	}
}
