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

	// 폰 가져오기
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	//AI 폰으로 변환
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

	// 델리게이트에 함수 등록
	FCainMontageFinished OnFinished;
	OnFinished.BindLambda(
		[&]()
		{
			// InProgress에서 이 태스크를 성공으로 마무리 짓고 다음 노드로 이동
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->SetMontageFinDelegate(OnFinished);

	// 공격
	AIPawn->PlayMontageByAI(EPattern::THROWAWAY);

	return EBTNodeResult::InProgress;
}
