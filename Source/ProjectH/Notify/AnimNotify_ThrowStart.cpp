// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotify_ThrowStart.h"
#include "Actor/Character/Cain.h"
#include "Components/CapsuleComponent.h"
#include "Data/HCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Character/PlayerCharacter.h"

void UAnimNotify_ThrowStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (ACain* AttackPawn = Cast<ACain>(MeshComp->GetOwner()))
		{
			if (APlayerCharacter* holdingPlayer = AttackPawn->HoldingPlayer)
			{
				holdingPlayer->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				holdingPlayer->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				holdingPlayer->GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_HCAPSULE);
				holdingPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
				holdingPlayer->SetActorRotation(FRotator::ZeroRotator);
				holdingPlayer->LaunchCharacter(AttackPawn->GetActorForwardVector() * 2000, true, true);
				AttackPawn->bAllowNextPattern = false;
				AttackPawn->HoldingPlayer = nullptr;
			}
		}
	}
}
