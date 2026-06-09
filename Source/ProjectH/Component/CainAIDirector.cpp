// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CainAIDirector.h"
#include "Actor/Character/Cain.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Interface/CainPatternInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CainAIKey.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Misc/ConfigCacheIni.h"

// Slate (우측 상단 디버그 위젯)
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Engine/GameViewportClient.h"
#include "Async/Async.h"

// ─── 콘솔 변수 (r.CainPatternMode 0=난수/1=AI대기/2=AI+폴백) ───────────
// 게임시작시 자동등록
static TAutoConsoleVariable<int32> CVarCainPatternMode(
	TEXT("r.CainPatternMode"),
	0,
	TEXT("0 = Classic(RandRange)\n")
	TEXT("1 = AI Director only (wait until LLM responds, no random fallback)\n")
	TEXT("2 = AI Director + RandRange fallback on failure/timeout"),
	ECVF_Cheat //치트,디버그용 (에디터 ,개발빌드)
);

UCainAIDirector::UCainAIDirector()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCainAIDirector::BeginPlay()
{
	Super::BeginPlay();
	UpdateDebugWidget(TEXT("[AI] READY"), TEXT(""), FLinearColor::White);

	//Mode1 에서는 대기상태로 시작
	if (CVarCainPatternMode.GetValueOnGameThread() == 1)
	{
		ApplyAIWaitSentinel();
	}
}

void UCainAIDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bRequestInFlight = false;
	CachedRequestAvailablePatterns.Empty();
	DestroyDebugWidget(); //위젯 미삭제시 null 에러발생가능
	Super::EndPlay(EndPlayReason);
}

//  우측 상단 Slate 디버그 위젯 생성
void UCainAIDirector::CreateDebugWidget()
{
	if (AIDebugTextBlock.IsValid())
	{
		return;
	}

	if (!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	//폰트 설정
	FSlateFontInfo FontInfo = FCoreStyle::Get().GetFontStyle("NormalFont");
	FontInfo.Size = 14;

	//슬레이트 위젯
	// 스마트 포인터로 관리, 수명관리직접해야함, UOBject가아님
	// 디버그용으로 많이 사용함, UMG에비해 간단한 UI 경우 만드는게 더 빠르고 가벼움
	//텍스트 설정
	SAssignNew(AIDebugTextBlock, STextBlock)
		.Text(FText::FromString(TEXT("[AI] READY")))
		.Font(FontInfo)
		.ColorAndOpacity(FLinearColor::White);

	//오버레이
	TSharedRef<SOverlay> Overlay =
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		.Padding(FMargin(0.f, 20.f, 20.f, 0.f))
		[
			AIDebugTextBlock.ToSharedRef()
		];

	AIDebugWidget = Overlay;
	GEngine->GameViewport->AddViewportWidgetContent(Overlay, 10);
}

void UCainAIDirector::DestroyDebugWidget()
{
	if (GEngine && GEngine->GameViewport && AIDebugWidget.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(AIDebugWidget.ToSharedRef());
	}

	AIDebugWidget.Reset();
	AIDebugTextBlock.Reset();
}

void UCainAIDirector::UpdateDebugWidget(const FString& Line1, const FString& Line2, FLinearColor Color)
{
	//텍스트 갱신 본문
	auto UpdateImpl = [this, Line1, Line2, Color]()
	{
		if (!AIDebugTextBlock.IsValid())
		{
			CreateDebugWidget();
		}
		if (!AIDebugTextBlock.IsValid())
		{
			return;
		}

		const FString Combined = Line2.IsEmpty()
			? Line1
			: FString::Printf(TEXT("%s\n%s"), *Line1, *Line2);

		AIDebugTextBlock->SetText(FText::FromString(Combined));
		AIDebugTextBlock->SetColorAndOpacity(Color);
	};

	//반드시 게임스레드에서 실행할것
	if (IsInGameThread())
	{
		UpdateImpl();
	}
	else
	{
		//다른 스레드면 다음 틱 게임스레드에서 실행 요청
		AsyncTask(ENamedThreads::GameThread, [this, UpdateImpl]()
		{
			if (IsValid(this))
			{
				UpdateImpl();
			}
		});
	}
}

// 플레이어 행동 기록
void UCainAIDirector::RecordAction(EPlayerAction Action)
{
	if (ActionBuffer.Num() >= ActionBufferSize)
	{
		ActionBuffer.RemoveAt(0);
	}
	ActionBuffer.Add(Action);

	// 연속 패링 카운트 갱신
	if (Action == EPlayerAction::Parry)
	{
		++ParryStreak;
	}
	else
	{
		ParryStreak = 0;
	}
}

// 다음 패턴 비동기 요청
void UCainAIDirector::RequestNextPatternAsync()
{
	// 콘솔변수 또는 컴포넌트 플래그 둘 다 체크
	const int32 PatternMode = CVarCainPatternMode.GetValueOnGameThread();
	const bool bAIMode = bUseAI || (PatternMode == 1) || (PatternMode == 2);
	if (!bAIMode)
	{
		return;
	}

	if (bRequestInFlight)
	{
		return;
	}

	// Rate Limit 방지: 쿨다운 중이면 그냥 리턴
	const double Now = FPlatformTime::Seconds();
	if ((Now - LastRequestCompletedTime) < MinRequestIntervalSeconds)
	{
		return;
	}

	// API 키 읽기 (DefaultGame.ini [CainAI] ApiKey=...)
	FString ApiKey;
	GConfig->GetString(TEXT("CainAI"), TEXT("ApiKey"), ApiKey, GGameIni);
	if (ApiKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[CainAI] ApiKey not found in DefaultGame.ini [CainAI] section. Falling back to RandRange."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[CainAI] ApiKey read OK. Length=%d, Prefix=%s"), ApiKey.Len(), *ApiKey.Left(6));

	FString ContextJson = BuildContextJson();
	UE_LOG(LogTemp, Log, TEXT("[CainAI][Input] %s"), *ContextJson);

	// Groq API 요청 조립 (OpenAI 호환 형식)
	const FString Url = TEXT("https://api.groq.com/openai/v1/chat/completions");

	// 프롬프트 조립
	FString Prompt = FString::Printf(
		TEXT("You are a boss AI director for an action game. "
			"Select the single best next attack pattern for the boss based on the context below. "
			"Reply with ONLY a JSON object in this exact format: {\"next_pattern\": \"<pattern_id>\", \"reason\": \"<one English word or short phrase>\"}\n\n"
			"Context:\n%s\n\n"
			"Rules:\n"
			"- Choose ONLY from available_patterns list\n"
			"- NEVER pick the same pattern as prev_pattern\n"
			"- If player parry_streak >= 2, prefer throwaway or hook2\n"
			"- If distance is far, prefer backdash or leftdash\n"
			"- Phase 1 near: vary between hook1, throwaway, smashhead, strongkick\n"
			"- Phase 2 near: vary between uppercut, hook2, stomp2\n"
			"- Output exactly one JSON object, nothing else."),
		*ContextJson
	);

	// Groq 요청 바디 (OpenAI chat completions 형식)
	TSharedPtr<FJsonObject> UserMessage = MakeShared<FJsonObject>();
	UserMessage->SetStringField(TEXT("role"), TEXT("user"));
	UserMessage->SetStringField(TEXT("content"), Prompt);

	TArray<TSharedPtr<FJsonValue>> Messages;
	Messages.Add(MakeShared<FJsonValueObject>(UserMessage));

	TSharedPtr<FJsonObject> RequestBody = MakeShared<FJsonObject>();
	RequestBody->SetStringField(TEXT("model"), TEXT("llama-3.1-8b-instant"));
	RequestBody->SetNumberField(TEXT("max_tokens"), 64);
	RequestBody->SetArrayField(TEXT("messages"), Messages);

	FString BodyString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
	FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);

	// HTTP 요청
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(Url);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *ApiKey));
	HttpRequest->SetContentAsString(BodyString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCainAIDirector::OnHttpResponse);

	CachedRequestAvailablePatterns = GetAvailablePatterns();

	bRequestInFlight = true;
	RequestStartTime = FPlatformTime::Seconds();
	HttpRequest->ProcessRequest();
}

FString UCainAIDirector::NormalizePatternId(FString PatternName)
{
	return PatternName.TrimStartAndEnd().ToLower();
}

// HTTP 응답 처리 
void UCainAIDirector::OnHttpResponse(
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request,
	TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response,
	bool bConnectedSuccessfully)
{
	if (!IsValid(this))
	{
		return;
	}

	bRequestInFlight = false;
	LastRequestCompletedTime = FPlatformTime::Seconds();
	const double ElapsedMs = (LastRequestCompletedTime - RequestStartTime) * 1000.0;

	//요청 실패
	if (!bConnectedSuccessfully || !Response.IsValid())
	{
		CachedRequestAvailablePatterns.Empty();
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] HTTP failed after %.1f ms. Using RandRange."), ElapsedMs);
		return;
	}

	//200 == 정상요청
	if (Response->GetResponseCode() != 200)
	{
		CachedRequestAvailablePatterns.Empty();
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] HTTP %d after %.1f ms. Body: %s. Using RandRange."),
			Response->GetResponseCode(), ElapsedMs, *Response->GetContentAsString().Left(200));
		return;
	}

	// Groq 응답에서 텍스트 추출 (OpenAI 형식)
	// 구조: choices[0].message.content
	FString ResponseBody = Response->GetContentAsString();
	TSharedPtr<FJsonObject> OuterJson;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
	if (!FJsonSerializer::Deserialize(Reader, OuterJson) || !OuterJson.IsValid())
	{
		CachedRequestAvailablePatterns.Empty();
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] Outer JSON parse failed. Using RandRange."));
		return;
	}

	FString PatternText;
	const TArray<TSharedPtr<FJsonValue>>* Choices;
	if (!OuterJson->TryGetArrayField(TEXT("choices"), Choices) || Choices->Num() == 0)
	{
		CachedRequestAvailablePatterns.Empty();
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] No choices in response. Using RandRange."));
		return;
	}

	const TSharedPtr<FJsonObject>* ChoiceObj;
	if (!(*Choices)[0]->TryGetObject(ChoiceObj))
	{
		CachedRequestAvailablePatterns.Empty();
		return;
	}

	const TSharedPtr<FJsonObject>* MessageObj;
	if (!(*ChoiceObj)->TryGetObjectField(TEXT("message"), MessageObj))
	{
		CachedRequestAvailablePatterns.Empty();
		return;
	}

	if (!(*MessageObj)->TryGetStringField(TEXT("content"), PatternText))
	{
		CachedRequestAvailablePatterns.Empty();
		return;
	}

	// 텍스트 안의 JSON 파싱 → next_pattern 추출
	TSharedPtr<FJsonObject> PatternJson;
	TSharedRef<TJsonReader<>> PatternReader = TJsonReaderFactory<>::Create(PatternText);
	if (!FJsonSerializer::Deserialize(PatternReader, PatternJson) || !PatternJson.IsValid())
	{
		CachedRequestAvailablePatterns.Empty();
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] Inner JSON parse failed: '%s'. Using RandRange."), *PatternText);
		return;
	}

	FString PatternName;
	if (!PatternJson->TryGetStringField(TEXT("next_pattern"), PatternName))
	{
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] next_pattern field missing. Using RandRange."));
		CachedRequestAvailablePatterns.Empty();
		return;
	}
	PatternName = NormalizePatternId(PatternName);

	// 유효성 검사: 요청 시점 available_patterns 기준 
	ACain* Cain = Cast<ACain>(GetOwner());
	ICainPatternInterface* CainInterface = Cast<ICainPatternInterface>(GetOwner());
	const bool bIsFirstPhase = CainInterface ? CainInterface->IsFirstPhase() : true;

	bool bPatternAllowed = false;
	for (const FString& Allowed : CachedRequestAvailablePatterns)
	{
		if (NormalizePatternId(Allowed) == PatternName)
		{
			bPatternAllowed = true;
			break;
		}
	}
	CachedRequestAvailablePatterns.Empty();

	if (!bPatternAllowed)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[CainAI][Fallback] Pattern '%s' not in request-time available_patterns. Using RandRange."),
			*PatternName);
		if (CVarCainPatternMode.GetValueOnGameThread() == 1)
		{
			ApplyAIWaitSentinel();
		}
		return;
	}

	int32 PatternNumber = PatternNameToRandomNumber(PatternName, bIsFirstPhase);
	if (PatternNumber < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] Pattern '%s' has no number mapping. Using RandRange."), *PatternName);
		if (CVarCainPatternMode.GetValueOnGameThread() == 1)
		{
			ApplyAIWaitSentinel();
		}
		return;
	}

	// 직전 패턴과 같으면 폐기
	if (CainInterface && PatternNumber == CainInterface->GetPrevRandomNumber())
	{
		UE_LOG(LogTemp, Warning, TEXT("[CainAI][Fallback] Pattern '%s' same as previous. Using RandRange."), *PatternName);
		if (CVarCainPatternMode.GetValueOnGameThread() == 1)
		{
			ApplyAIWaitSentinel();
		}
		return;
	}

	CachedPatternNumber = PatternNumber;
	CachedPatternName = PatternName;
	PatternJson->TryGetStringField(TEXT("reason"), CachedReason);

	UE_LOG(LogTemp, Log, TEXT("[CainAI][Selected] pattern=%s number=%d reason=%s elapsed=%.1f ms"),
		*PatternName, PatternNumber, *CachedReason, ElapsedMs);

	//위젯 업데이트
	UpdateDebugWidget(
		FString::Printf(TEXT("[AI] %s"), *PatternName.ToUpper()),
		FString::Printf(TEXT("%s  (%.0fms)"), *CachedReason, ElapsedMs),
		FLinearColor::Green
	);

	// Mode 1: idle일 때만 BB+Prev 동시 반영. 공격 중이면 캐시만 두고 MontageEnd/BTS가 소비.
	const int32 PatternMode = CVarCainPatternMode.GetValueOnGameThread();
	ICainPatternInterface* CainIface = Cast<ICainPatternInterface>(GetOwner());
	if (PatternMode == 1 && CainIface && CainIface->AllowNextPattern())
	{
		CommitPatternToBlackboard(PatternNumber);
		CachedPatternNumber = -1;
		UE_LOG(LogTemp, Log, TEXT("[CainAI] Mode=1 committed %d to BB (prev synced)"), PatternNumber);
	}
	else if (PatternMode == 1)
	{
		UE_LOG(LogTemp, Log, TEXT("[CainAI] Mode=1 cached %d — waiting for montage end"), PatternNumber);
	}
	else if (CainIface && CainIface->AllowNextPattern())
	{
		CommitPatternToBlackboard(PatternNumber);
		CachedPatternNumber = -1;
		UE_LOG(LogTemp, Log, TEXT("[CainAI] Wrote %d to BB immediately (no BTS tick wait)"), PatternNumber);
	}
}

void UCainAIDirector::ApplyAIWaitSentinel()
{
	ICainPatternInterface* CainIface = Cast<ICainPatternInterface>(GetOwner());
	if (CainIface)
	{
		CainIface->SetPrevRandomNumber(BB_IDLE_RANDOMNUMBER);
	}

	ACain* CainOwner = Cast<ACain>(GetOwner());
	if (CainOwner && CainOwner->GetController())
	{
		if (AAIController* AIC = Cast<AAIController>(CainOwner->GetController()))
		{
			if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
			{
				BB->SetValueAsInt(BBKEY_RANDOMNUMBER, BB_IDLE_RANDOMNUMBER);
			}
		}
	}
}

bool UCainAIDirector::HasCachedAIPattern() const
{
	return CachedPatternNumber >= 1 || bFreshAIPatternInBB;
}

void UCainAIDirector::CommitPatternToBlackboard(int32 PatternNumber)
{
	ICainPatternInterface* CainIface = Cast<ICainPatternInterface>(GetOwner());
	if (CainIface)
	{
		CainIface->SetPrevRandomNumber(PatternNumber);
	}

	ACain* CainOwner = Cast<ACain>(GetOwner());
	if (CainOwner && CainOwner->GetController())
	{
		if (AAIController* AIC = Cast<AAIController>(CainOwner->GetController()))
		{
			if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
			{
				BB->SetValueAsInt(BBKEY_RANDOMNUMBER, PatternNumber);
			}
		}
	}

	bFreshAIPatternInBB = true;
}

// 패턴 시작 알림
// PlayMontageByAI에서 호출. AI가 선택한 패턴 몽타주가 시작되면 fresh 플래그 클리어.
void UCainAIDirector::NotifyPatternStarted()
{
	if (!bFreshAIPatternInBB)
	{
		// 콤보 연속기 패턴 — BB 유지
		return;
	}

	bFreshAIPatternInBB = false;
	UE_LOG(LogTemp, Log, TEXT("[CainAI] NotifyPatternStarted: AI pattern montage started"));
}

//  BTS에서 캐시 소비
int32 UCainAIDirector::ConsumeCachedPatternNumber()
{
	// 타임아웃 체크
	if (CachedPatternNumber >= 0 && bRequestInFlight)
	{
		const double Elapsed = FPlatformTime::Seconds() - RequestStartTime;
		if (Elapsed > TimeoutSeconds)
		{
			bRequestInFlight = false;
			UE_LOG(LogTemp, Warning, TEXT("[CainAI][Timeout] %.1f s exceeded. Using RandRange."), (float)Elapsed);
			return -1;
		}
	}

	const int32 Result = CachedPatternNumber;
	CachedPatternNumber = -1; // 소비 후 초기화
	return Result;
}

//  컨텍스트 JSON 빌드
FString UCainAIDirector::BuildContextJson() const
{
	ACain* Cain = Cast<ACain>(GetOwner());
	ICainPatternInterface* CainInterface = Cast<ICainPatternInterface>(GetOwner());
	if (!Cain || !CainInterface) return TEXT("{}");

	// HP%
	float HpPct = 100.0f;
	if (Cain->GetMaxHP() > 0.0f)
	{
		HpPct = (Cain->GetCurrentHP() / Cain->GetMaxHP()) * 100.0f;
	}

	// 페이즈
	const int32 Phase = CainInterface->IsFirstPhase() ? 1 : 2;

	// 거리
	EDistanceZone Zone = GetDistanceZone();
	FString DistStr = DistanceZoneToString(Zone);

	// 최근 행동
	TArray<FString> ActionStrs;
	for (EPlayerAction A : ActionBuffer)
	{
		ActionStrs.Add(ActionToString(A));
	}

	// available_patterns
	TArray<FString> Available = GetAvailablePatterns();

	// 이전 패턴 이름 (반복 방지용)
	FString PrevPatternName = TEXT("none");
	if (CainInterface->GetPrevRandomNumber() > 0)
	{
		PrevPatternName = CachedPatternName.IsEmpty() ? TEXT("unknown") : CachedPatternName;
	}

	// JSON 조립
	TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
	Json->SetNumberField(TEXT("boss_hp_pct"), FMath::RoundToFloat(HpPct * 10.0f) / 10.0f);
	Json->SetNumberField(TEXT("phase"), Phase);
	Json->SetStringField(TEXT("prev_pattern"), PrevPatternName);
	Json->SetStringField(TEXT("distance"), DistStr);
	Json->SetNumberField(TEXT("parry_streak"), ParryStreak);

	TArray<TSharedPtr<FJsonValue>> ActionArr;
	for (const FString& S : ActionStrs)
	{
		ActionArr.Add(MakeShared<FJsonValueString>(S));
	}
	Json->SetArrayField(TEXT("player_recent_actions"), ActionArr);

	TArray<TSharedPtr<FJsonValue>> PatternArr;
	for (const FString& P : Available)
	{
		PatternArr.Add(MakeShared<FJsonValueString>(P));
	}
	Json->SetArrayField(TEXT("available_patterns"), PatternArr);

	FString Out;
	TSharedRef<TJsonWriter<>> W = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(Json.ToSharedRef(), W);
	return Out;
}

// 거리 계산
EDistanceZone UCainAIDirector::GetDistanceZone() const
{
	ACain* Cain = Cast<ACain>(GetOwner());
	if (!Cain) return EDistanceZone::Mid;

	AAIController* AIC = Cast<AAIController>(Cain->GetController());
	if (!AIC) return EDistanceZone::Mid;

	UBlackboardComponent* BB = AIC->GetBlackboardComponent();
	if (!BB) return EDistanceZone::Mid;

	// BB Distance key 활용
	const float Dist = BB->GetValueAsFloat(BBKEY_DISTANCE);
	if (Dist < 500.0f) return EDistanceZone::Near;
	if (Dist < 1000.0f) return EDistanceZone::Mid;
	return EDistanceZone::Far;
}

FString UCainAIDirector::DistanceZoneToString(EDistanceZone Zone) const
{
	switch (Zone)
	{
	case EDistanceZone::Near: return TEXT("near");
	case EDistanceZone::Mid:  return TEXT("mid");
	case EDistanceZone::Far:  return TEXT("far");
	}
	return TEXT("mid");
}

FString UCainAIDirector::ActionToString(EPlayerAction Action) const
{
	switch (Action)
	{
	case EPlayerAction::Parry: return TEXT("parry");
	case EPlayerAction::Roll:  return TEXT("roll");
	case EPlayerAction::Hit:   return TEXT("hit");
	}
	return TEXT("unknown");
}

// 현재 상황에서 사용 가능한 패턴 목록 
TArray<FString> UCainAIDirector::GetAvailablePatterns() const
{
	ICainPatternInterface* CainInterface = Cast<ICainPatternInterface>(GetOwner());
	const bool bIsFirstPhase = CainInterface ? CainInterface->IsFirstPhase() : true;
	EDistanceZone Zone = GetDistanceZone();

	TArray<FString> Available;

	if (Zone == EDistanceZone::Far)
	{
		// BackDash(<50), LeftDash(<100) 확인됨
		Available = { TEXT("backdash"), TEXT("leftdash") };
	}
	else if (Zone == EDistanceZone::Mid)
	{
		Available = bIsFirstPhase
			? TArray<FString>{ TEXT("backdash"), TEXT("leftdash") }
			: TArray<FString>{ TEXT("backdash"), TEXT("leftdash") };
	}
	else // Near
	{
		if (bIsFirstPhase)
			// punch1/grab은 연속기(combo) → AI가 새 패턴으로 선택하면 안 됨
			// hook1/throwaway가 각 구간의 새 패턴 시작자
			Available = { TEXT("hook1"), TEXT("throwaway"), TEXT("smashhead"), TEXT("strongkick") };
		else
			Available = { TEXT("uppercut"), TEXT("hook2"), TEXT("stomp2") };
	}

	return Available;
}

// 패턴 이름 → RandomNumber 변환
// BT 확률 구간의 중간값을 사용, BT 분기를 변경하지 않음
int32 UCainAIDirector::PatternNameToRandomNumber(const FString& PatternName, bool bIsFirstPhase) const
{
	if (bIsFirstPhase)
	{
		// 1페이즈 실제 BT 구조:
		// hook1     : RandomNumber < 55        → 28 (새 패턴 시작자)
		// throwaway : RandomNumber 55~74       → 65 (새 패턴 시작자)
		// smashhead : RandomNumber 75~84       → 80
		// strongkick: RandomNumber 85~99       → 92
		// 원거리 대시 (Distance < 400):
		// backdash  : RandomNumber < 5         → 3
		// leftdash  : RandomNumber < 10        → 7
		if (PatternName == TEXT("hook1"))      return 28;
		if (PatternName == TEXT("throwaway"))  return 65;
		if (PatternName == TEXT("smashhead"))  return 80;
		if (PatternName == TEXT("strongkick")) return 92;
		if (PatternName == TEXT("backdash"))   return 3;
		if (PatternName == TEXT("leftdash"))   return 7;
	}
	else
	{
		// 2페이즈 BT 실제 구조:
		// uppercut  : RandomNumber < 40        → 20
		// hook2     : RandomNumber 40~70       → 55
		// stomp2    : RandomNumber > 70 AND < 100 → 85
		if (PatternName == TEXT("uppercut"))   return 20;
		if (PatternName == TEXT("hook2"))      return 55;
		if (PatternName == TEXT("stomp2"))     return 85;
		if (PatternName == TEXT("backdash"))   return 3;
		if (PatternName == TEXT("leftdash"))   return 7;
	}

	return -1; // 알 수 없는 패턴
}
