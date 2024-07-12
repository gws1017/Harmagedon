#include "Actor/Item/Potion/HealPotion.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "Components/StaticMeshComponent.h"

AHealPotion::AHealPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");
}

void AHealPotion::Use()
{
	CheckNull(DrinkMontage);
	auto Player = Cast<APlayerCharacter>(GetOwner());
	auto AnimInstance = Player->GetMesh()->GetAnimInstance();
	
	//�޽� ����
	AttachToComponent(Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "PotionSocket");

	AnimInstance->Montage_Play(DrinkMontage);
	AnimInstance->Montage_JumpToSection("DrinkSucc");

	//��Ÿ�� ����� �޽� �ı�
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {this->Destroy(); });
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DrinkMontage);
}

void AHealPotion::UseFail()
{
	SetActorHiddenInGame(true);
	CheckNull(DrinkMontage);
	auto Player = Cast<APlayerCharacter>(GetOwner());
	auto AnimInstance = Player->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DrinkMontage);
	AnimInstance->Montage_JumpToSection("DrinkFail");
	Destroy();
}
