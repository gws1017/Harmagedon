#include "Actor/Item/Armor.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

AArmor::AArmor()
{
}

void AArmor::BeginPlay()
{
	Super::BeginPlay();
}

void AArmor::Equip(EEquipType Type)
{
	Super::Equip(Type);
	InitializeItemData();

	auto Player = Cast<APlayerCharacter>(GetOwnerCharacter());
	TMap<EEquipType, AArmor*>& EquippedArmor = Player->GetArmors();
	TArray<UMeshComponent*> ArmorArray = Player->GetArmorComponent(Type);

	switch (Type)
	{
	case EEquipType::ET_Head:
	{
		CheckNull(GetStaticMeshArray().Num() > 0);
		for (int i = 0; i < ArmorArray.Num(); ++i)
			Cast<UStaticMeshComponent>(ArmorArray[i])->SetStaticMesh(GetStaticMeshArray()[i]);
		break;
	}
	case EEquipType::ET_Top:
	{
		CheckNull(GetSkeletalMesh());
		CheckNull(GetStaticMeshArray().Num() > 0);
		auto Component = Cast<USkeletalMeshComponent>(ArmorArray[0]);
		Component->SetSkeletalMesh(GetSkeletalMesh());
		Component->SetLeaderPoseComponent(Player->GetMesh());
		for (int i = 1; i < ArmorArray.Num(); ++i)
			Cast<UStaticMeshComponent>(ArmorArray[i])->SetStaticMesh(GetStaticMeshArray()[i - 1]);
		break;
	}
	case EEquipType::ET_Bottom:
	{
		CheckNull(GetSkeletalMesh());
		auto Component = Cast<USkeletalMeshComponent>(ArmorArray[0]);
		Component->SetSkeletalMesh(GetSkeletalMesh());
		Component->SetLeaderPoseComponent(Player->GetMesh());
		break;
	}
	case EEquipType::ET_Hand:
	{
		CheckNull(GetStaticMeshArray().Num() > 0);
		for (int i = 0; i < ArmorArray.Num(); ++i)
			Cast<UStaticMeshComponent>(ArmorArray[i])->SetStaticMesh(GetStaticMeshArray()[i]);
		break;
	}
	case EEquipType::ET_Shoe:
	{
		CheckNull(GetSkeletalMesh());
		auto Component = Cast<USkeletalMeshComponent>(ArmorArray[0]);
		Component->SetSkeletalMesh(GetSkeletalMesh());
		Component->SetLeaderPoseComponent(Player->GetMesh());
		break;
	}
	}
	if (EquippedArmor.Contains(Type))
		EquippedArmor[Type] = this;
	else
		EquippedArmor.Add({ Type,this });
}

void AArmor::UnEquip(EEquipType Type)
{
	Super::UnEquip(Type);

	auto Player = Cast<APlayerCharacter>(GetOwnerCharacter());

	TArray<UMeshComponent*> ArmorArray = Player->GetArmorComponent(Type);

	auto& EquippedArmor = Player->GetArmors();
	EquippedArmor[Type] = nullptr;

	for (auto ArmorComponent : ArmorArray)
	{
		auto SkeletalArmor = Cast<USkeletalMeshComponent>(ArmorComponent);
		if (SkeletalArmor)
			SkeletalArmor->SetSkeletalMesh(nullptr);
		auto StaticArmor = Cast<UStaticMeshComponent>(ArmorComponent);
		if (StaticArmor)
			StaticArmor->SetStaticMesh(nullptr);
	}
}
