// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTS_CainDetect.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CainAIKey.h"
#include "Data/HCollision.h"
#include "Global.h"

UBTS_CainDetect::UBTS_CainDetect()
{
	NodeName = TEXT("CainDetect");
	Interval = 1.0f;
}

void UBTS_CainDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 비헤이비어 트리 소유한 폰 갖고오기
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	// 폰 위치와 폰이 속한 월드 가져오기
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	// 폰을 AI폰으로 변환
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISFIRSTPHASE, AIPawn->IsFirstPhase());

	// AI폰의 감지범위 반지름
	float DetectRadius = AIPawn->GetAIDetectRoomRange();

	TArray<FOverlapResult> OverlapResults;
	// 모든 폰 감지하기
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_HACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			// 폰들 중에 우리 플레이어인 경우
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// 블랙보드의 타겟값 저장 후 녹색으로 표시
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_ROOMTARGET, Pawn);

				// 거리
				float Distance = FVector::Distance(ControllingPawn->GetTargetLocation(), Pawn->GetTargetLocation());
				OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_DISTANCE, Distance);
				//CLog::Print(Distance);


				// 감지 영역 표시
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// 플레이어와의 위치를 선과 점으로 표시
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_ROOMTARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}