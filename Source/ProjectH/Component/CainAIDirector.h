// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widgets/SWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "CainAIDirector.generated.h"

// 플레이어 행동 타입
UENUM()
enum class EPlayerAction : uint8
{
	Parry,
	Roll,
	Hit,   // 카인이 플레이어에게 피격당함
};

// 거리 단계
UENUM()
enum class EDistanceZone : uint8
{
	Near,  // < 500
	Mid,   // 500~1000
	Far,   // > 1000
};

/**
 * UCainAIDirector
 * ACain에 부착되는 LLM 패턴 선택 컴포넌트
 * bUseAI=true 시 LLM에 비동기 요청, 캐시된 패턴을 BTS에 제공
 * 실패/타임아웃 시 무조건 -1 반환 → BTS가 RandRange로 폴백
 * Claude 기반 생성 코드
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTH_API UCainAIDirector : public UActorComponent
{
	GENERATED_BODY()

public:
	UCainAIDirector();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 외부 호출 API

	// 플레이어 행동 기록 (링버퍼, 최대 ActionBufferSize개)
	// 꽉차면 가장오래된 것을 버리고 새것을 넣는 구조 최근 N개 기억
	void RecordAction(EPlayerAction Action);

	// 다음 패턴 비동기 요청 시작 (패턴 실행 시작 시점에 호출)
	void RequestNextPatternAsync();

	// BTS에서 호출: 캐시 유효하면 RandomNumber 반환, 없으면 -1(폴백 신호)
	int32 ConsumeCachedPatternNumber();

	// Mode 1 대기 중인지 
	bool IsRequestInFlight() const { return bRequestInFlight; }

	// Rate limit 쿨다운 중인지
	bool IsRateLimited() const
	{
		return (FPlatformTime::Seconds() - LastRequestCompletedTime) < MinRequestIntervalSeconds;
	}

	// AI가 BB에 값을 기록한 직후 BTS가 덮어쓰지 않도록 보호하는 플래그
	// BTS 캐시 소비 시 또는 OnHttpResponse 즉시 쓰기 시 true로 설정
	// NotifyPatternStarted에서 AI 패턴이 실제 시작될 때 false로 클리어
	bool IsFreshAIPatternInBB() const { return bFreshAIPatternInBB; }
	void SetFreshAIPattern(bool bFresh) { bFreshAIPatternInBB = bFresh; }

	// Cain::PlayMontageByAI에서 호출 — AI 패턴 몽타주 시작 시 fresh 플래그 클리어
	void NotifyPatternStarted();

	// Mode 1: AI 명령 대기 — BB/Prev를 idle sentinel으로 (BT 패턴 분기 진입 차단)
	void ApplyAIWaitSentinel();

	// 캐시·fresh BB에 실행 대기 패턴이 있는지
	bool HasCachedAIPattern() const;

	// BB 갱신 
	void CommitPatternToBlackboard(int32 PatternNumber);

	// 우측 상단 디버그 위젯 텍스트 갱신 (BTS에서도 호출)
	void UpdateDebugWidget(const FString& Line1, const FString& Line2, FLinearColor Color);

	// 설정

	// true = AI 모드, false = 기존 난수  — 콘솔변수로도 제어됨
	UPROPERTY(EditAnywhere, Category = "AI Director")
	bool bUseAI = false;

	// 타임아웃 초 (이 시간 내 응답 없으면 캐시 무효 처리)
	UPROPERTY(EditAnywhere, Category = "AI Director")
	float TimeoutSeconds = 2.5f;

private:

	//  컨텍스트 빌드
	FString BuildContextJson() const;
	FString DistanceZoneToString(EDistanceZone Zone) const;
	FString ActionToString(EPlayerAction Action) const;
	TArray<FString> GetAvailablePatterns() const;
	EDistanceZone GetDistanceZone() const;

	// HTTP 응답 처리
	void OnHttpResponse(
		TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> Request,
		TSharedPtr<class IHttpResponse, ESPMode::ThreadSafe> Response,
		bool bConnectedSuccessfully);

	// 패턴 이름 → RandomNumber 변환 (BT 구간 중간값)
	// 유효하지 않은 패턴이면 -1 반환
	int32 PatternNameToRandomNumber(const FString& PatternName, bool bIsFirstPhase) const;

	static FString NormalizePatternId(FString PatternName);

	// 상태
	// 플레이어 행동 링버퍼
	static constexpr int32 ActionBufferSize = 8;
	TArray<EPlayerAction> ActionBuffer;

	// 연속 패링 카운트
	int32 ParryStreak = 0;

	// 캐시된 패턴 RandomNumber (-1 = 캐시 없음)
	int32 CachedPatternNumber = -1;

	// 캐시된 패턴 이름 + 선택 이유 (화면 표시용)
	FString CachedPatternName;
	FString CachedReason;

	// 현재 HTTP 요청 진행 중 여부 (중복 요청 방지)
	bool bRequestInFlight = false;

	// AI가 BB에 값을 쓴 직후 true — 콤보 패턴이 BB를 덮어쓰지 않도록 보호
	// BTS가 valid BB를 보존할 때만 사용; AI 패턴 실제 실행 시 false로 클리어
	bool bFreshAIPatternInBB = false;

	// 요청 발사 시각 (타임아웃 체크용)
	double RequestStartTime = 0.0;

	// 마지막 요청 완료 시각 (Rate Limit 방지용)
	double LastRequestCompletedTime = 0.0;

	// 최소 요청 간격 (초)
	static constexpr double MinRequestIntervalSeconds = 5.0;

	// 요청 시점의 available_patterns (응답 시 거리 변동으로 재검증 실패 방지)
	TArray<FString> CachedRequestAvailablePatterns;

	// 우측 상단 디버그 UI 
	TSharedPtr<STextBlock> AIDebugTextBlock;
	TSharedPtr<SWidget>    AIDebugWidget;

	void CreateDebugWidget();
	void DestroyDebugWidget();
};
