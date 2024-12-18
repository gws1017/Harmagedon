// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotify_CainParryCheck.h"
#include "Interface/CainAnimNotifyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Actor/Character/Cain.h"
#include "Actor/Character/PlayerCharacter.h"
#include "AIModule/Classes/AIController.h"
#include "Data/CainAIKey.h"

void UAnimNotify_CainParryCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ACain* BossCain = Cast<ACain>(MeshComp->GetOwner());
		if (BossCain)
		{
			AAIController* AIController = Cast<AAIController>(BossCain->GetController());
			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
			if (BlackboardComp)
			{
				APlayerCharacter* playercharacter = Cast<APlayerCharacter>(BlackboardComp->GetValueAsObject(BBKEY_ROOMTARGET));
				// 패링성공시
				if (playercharacter->GetParrySucc())
				{
					BossCain->IsSuccessParry(true);
					//보스 애니메이션 중지
					BossCain->StopAnim();
				}
			}
		}
	}
}
