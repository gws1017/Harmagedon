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
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
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

void AWeapon::PlayAttackMontage()
{
	CheckNull(AttackMontage);
	auto Player = Cast<APlayerCharacter>(OwnerCharacter);
	if (!Player)
	{
		OwnerCharacter->PlayAnimMontage(AttackMontage);
	}
	else
	{
		Player->PlayAttackMontage(EquipType);
	}
}

void AWeapon::WeaponApplyDamage(AActor* OtherActor)
{
	//자기자신 제외
	IgnoreActors.AddUnique(Owner);

	//서로 같은 그룹이면 비교하지 않는다
	if (IsSameTagWithTarget(OtherActor, "Enemy")) return;
	if (IsSameTagWithTarget(OtherActor, "Player")) return;

	//물리 충격을 가함
	CreateField(GetActorLocation());

	if (!!OtherActor && !IgnoreActors.Contains(OtherActor))
	{
		IHitInterface* other = Cast<IHitInterface>(OtherActor);

		CheckNull(other);
		float AdditionalDamage = 0.f;
		if (Owner->ActorHasTag("Player"))
			AdditionalDamage = Cast<APlayerCharacter>(Owner)->GetStrDamage();

		IgnoreActors.AddUnique(OtherActor);

		other->Hit(GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, Damage + AdditionalDamage, WeaponInstigator, Owner, DamageTypeClass);
	}
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
	else
	{
		CLog::Log("DrawMontage is Not Set!");
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

void AWeapon::Block()
{
	//CLog::Log(GetName() + " Block");
}

void AWeapon::StrongAttack()
{
	CLog::Log(GetName() + " Strong Attack");
}

void AWeapon::SpecialAttack_Implementation()
{
	CLog::Log(GetName() + "This is Special Attack");
}
