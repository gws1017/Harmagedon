// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTS_DrawRandomNumber.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h" // FMath 헤더 포함
#include "Data/CainAIKey.h"

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

	AIPawn->SetPrevRandomNumber(RandomNumber);
	RandomNumber = FMath::RandRange(1, 100);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_RANDOMNUMBER, RandomNumber);
}
