#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Data/ItemData.h"
#include "Global.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

#include "Animation/AnimMontage.h"

#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"

AWeapon::AWeapon()
	:Damage(5), StaminaCost(10),
	RadialFalloffMagnitude(1000000.f), RadialVectorMagnitude(15000000.f)
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");

	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", Scene);
	UHelpers::CreateComponent<UBoxComponent>(this, &WeaponCollision, "ComatCollision", Scene);
	UHelpers::CreateComponent<UFieldSystemComponent>(this, &FieldSystemComponent, "FieldSystemComponent", GetRootComponent());

	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));
	MetaData = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("MetaData"));

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BoxBeginOverlap);

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
	//이 무기를 가진 캐릭터의 컨트롤러를 등록함
	SetInstigator(OwnerCharacter->GetController());

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SheathSocket);

}



bool AWeapon::IsSameTagWithTarget(AActor* other, const FName& tag)
{
	return Owner->ActorHasTag(tag) && other->ActorHasTag(tag);
}

void AWeapon::ActivateCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	IgnoreActors.Empty();
}

void AWeapon::DeactivateCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::CreateField(const FVector& FieldLocation)
{
	MetaData->ObjectType = EFieldObjectType::Field_Object_Destruction;
	RadialFalloff->SetRadialFalloff(RadialFalloffMagnitude, 0.8f, 1.0f, 0.f, 200.f, FieldLocation, Field_Falloff_Linear);
	RadialVector->SetRadialVector(RadialVectorMagnitude, FieldLocation);

	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearForce, MetaData, RadialVector);

	DrawDebugSphere(GetWorld(), FieldLocation, 25.f, 12, FColor::White, true, 30.f, 0, 1.f);
}

void AWeapon::Equip(EEquipType Type)
{
	CheckTrue(bEquipped);//이미 착용중이면 함수 종료
	CheckTrue(bEquipping);//무기 꺼내는중이여도 종료
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

	SetInstigator(OwnerCharacter->GetController()); //무기 변경 시 컨틀롤러 재등록 고려

}

void AWeapon::UnEquip(EEquipType Type)
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);//무기 넣는중이여도 종료

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

void AWeapon::Begin_Equip()
{
	bEquipped = true;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), EquipSocket);
}

void AWeapon::End_Equip()
{
	bEquipping = false;

}

void AWeapon::Begin_UnEquip()
{
	bEquipped = false;

}

void AWeapon::End_UnEquip()
{
	bEquipping = false;
	Destroy();
	//AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SheathSocket);
}

void AWeapon::Begin_Collision()
{
	ActivateCollision();
}

void AWeapon::End_Collision()
{
	DeactivateCollision();
}

void AWeapon::BasicAttack()
{
	CLog::Log(GetName() + " Basic Attack");
}

void AWeapon::StrongAttack()
{
	CLog::Log(GetName() + " Strong Attack");
}

void AWeapon::SpecialAttack_Implementation()
{
	CLog::Log(GetName() + "This is Special Attack");
}
