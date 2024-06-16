// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotify_CainSplashDamage.h"
#include "Actor/Character/Cain.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Engine/DamageEvents.h"

void UAnimNotify_CainSplashDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ACain* AttackPawn = Cast<ACain>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			float radius = 600;
			FVector start = AttackPawn->GetActorLocation();
			FVector end = AttackPawn->GetActorLocation() + AttackPawn->GetActorForwardVector() * 600 * 2;

			TArray<AActor*> HitActors;
			//DrawDebugSphere(GetWorld(), AttackPawn->GetActorLocation(), radius, 12, FColor::White, true, 3.f, 0, 1.f);

			if (AttackPawn->IsHitActorAreaAttack(start, end, radius, HitActors))
			{
				for (AActor* HitActor : HitActors)
				{
					// 데미지 전달
					HitActor->TakeDamage(300.0f, FDamageEvent(), AttackPawn->GetController(), AttackPawn);
				}
			}
		}
	}
}

