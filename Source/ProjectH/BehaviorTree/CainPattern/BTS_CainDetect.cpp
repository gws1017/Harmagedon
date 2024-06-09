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

	// �����̺�� Ʈ�� ������ �� �������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	// �� ��ġ�� ���� ���� ���� ��������
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	// ���� AI������ ��ȯ
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISFIRSTPHASE, AIPawn->IsFirstPhase());

	// AI���� �������� ������
	float DetectRadius = AIPawn->GetAIDetectRoomRange();

	TArray<FOverlapResult> OverlapResults;
	// ��� �� �����ϱ�
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
			// ���� �߿� �츮 �÷��̾��� ���
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// �������� Ÿ�ٰ� ���� �� ������� ǥ��
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_ROOMTARGET, Pawn);

				// �Ÿ�
				float Distance = FVector::Distance(ControllingPawn->GetTargetLocation(), Pawn->GetTargetLocation());
				OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_DISTANCE, Distance);
				//CLog::Print(Distance);


				// ���� ���� ǥ��
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// �÷��̾���� ��ġ�� ���� ������ ǥ��
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_ROOMTARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}