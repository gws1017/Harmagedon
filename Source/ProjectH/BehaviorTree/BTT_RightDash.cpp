// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTT_RightDash.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CainAIKey.h"

UBTT_RightDash::UBTT_RightDash()
{
}

EBTNodeResult::Type UBTT_RightDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// �� ��������
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	//AI ������ ��ȯ
	ICainPatternInterface* AIPawn = Cast<ICainPatternInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// ��������Ʈ�� �Լ� ���
	FCainMontageFinished OnFinished;
	OnFinished.BindLambda(
		[&]()
		{
			// InProgress���� �� �½�ũ�� �������� ������ ���� ���� ���� �̵�
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->SetMontageFinDelegate(OnFinished);

	// ����
	AIPawn->PlayMontageByAI(EMontages::RIGHTDASH);

	return EBTNodeResult::InProgress;
}
