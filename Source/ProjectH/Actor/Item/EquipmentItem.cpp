#include "Actor/Item/EquipmentItem.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"

#include "Animation/AnimMontage.h"

AEquipmentItem::AEquipmentItem()
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", Scene);
}

void AEquipmentItem::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void AEquipmentItem::Equip(EEquipType Type)
{

	CheckTrue(bEquipped);//�̹� �������̸� �Լ� ����
	CheckTrue(bEquipping);//���� ���������̿��� ����
	if (OwnerCharacter == nullptr)
	{
		CLog::Log("Weapon Owner is Nullptr!");
		return;
	}
	EquipType = Type;
	SetSocketName(Type);
	bEquipping = true;

	if (DrawMontage)
	{
		auto AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		FString SectionName;

		if (Type == EEquipType::ET_LeftWeapon)
			SectionName = "Left";
		if (Type == EEquipType::ET_RightWeapon)
			SectionName = "Right";

		AnimInstance->Montage_Play(DrawMontage);
		AnimInstance->Montage_JumpToSection(FName(SectionName));
	}
	else
	{
		CLog::Log("DrawMontage is Not Set!");
	}
}

void AEquipmentItem::UnEquip(EEquipType Type)
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);//���� �ִ����̿��� ����

	bEquipping = true;

	if (SheathMontage)
	{
		auto AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		FString SectionName;

		if (Type == EEquipType::ET_LeftWeapon)
			SectionName = "Left";
		if (Type == EEquipType::ET_RightWeapon)
			SectionName = "Right";

		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName(SectionName));
	}
	EquipType = EEquipType::ET_None;
}

void AEquipmentItem::Begin_Equip()
{
	bEquipped = true;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), EquipSocket);
}

void AEquipmentItem::End_Equip()
{
	bEquipping = false;
}

void AEquipmentItem::Begin_UnEquip()
{
	bEquipped = false;
}

void AEquipmentItem::End_UnEquip()
{
	bEquipping = false;
	Destroy();
}
