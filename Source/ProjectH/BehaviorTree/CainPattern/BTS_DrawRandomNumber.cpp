// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTS_DrawRandomNumber.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h" // FMath ��� ����
#include "Data/CainAIKey.h"

UBTS_DrawRandomNumber::UBTS_DrawRandomNumber()
{
	NodeName = TEXT("DrawRandomNumber");
	Interval = 1.0f;
}

void UBTS_DrawRandomNumber::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// �����̺�� Ʈ�� ������ �� �������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return;
	}

	// �� ��ġ�� ���� ���� ���� ��������
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (!World)
	{
		return;
	}

	// ���� AI������ ��ȯ
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return;
	}

	AIPawn->SetPrevRandomNumber(RandomNumber);
	RandomNumber = FMath::RandRange(1, 100);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_RANDOMNUMBER, RandomNumber);
}
