// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CainPattern/BTT_CainThrowAway.h"
#include "AIController.h"
#include "Interface/CainPatternInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CainAIKey.h"

#include "Actor/Character/Cain.h"
#include "Components/CapsuleComponent.h"
#include "Data/HCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Character/PlayerCharacter.h"

UBTT_CainThrowAway::UBTT_CainThrowAway()
{
}

EBTNodeResult::Type UBTT_CainThrowAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


	//APlayerCharacter* playerActor = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ROOMTARGET));

	//playerActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//playerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//playerActor->GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_HCAPSULE);
	//playerActor->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	//playerActor->SetActorRotation(FRotator::ZeroRotator);
	//playerActor->LaunchCharacter(ControllingPawn->GetActorForwardVector() * 2000, true, true);

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
	AIPawn->PlayMontageByAI(EPattern::THROWAWAY);

	return EBTNodeResult::InProgress;
}
