#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Character/PlayerCharacter.h"

#include "System/Sound/SoundManager.h"

#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

#include "Animation/AnimMontage.h"

#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"

#include "NiagaraComponent.h"

AWeapon::AWeapon()
	:Damage(5), StaminaCost(10),
	RadialFalloffMagnitude(1000000.f), RadialVectorMagnitude(15000000.f)
{
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", Scene);
	UHelpers::CreateComponent<UBoxComponent>(this, &WeaponCollision, "ComatCollision", Scene);
	UHelpers::CreateComponent<UFieldSystemComponent>(this, &FieldSystemComponent, "FieldSystemComponent", GetRootComponent());
	UHelpers::CreateComponent<UNiagaraComponent>(this, &NSParticleComponent, "NSComponent", Scene);

	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));
	MetaData = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("MetaData"));

	NSParticleComponent->SetAutoActivate(false);

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

	CheckNull(GetOwnerCharacter());
	//이 무기를 가진 캐릭터의 컨트롤러를 등록함
	SetInstigator(GetOwnerCharacter()->GetController());

	AttachToComponent(GetOwnerCharacter()->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SheathSocket);

	Damage = ItemData->StatData.PhysicalDamage;

}

void AWeapon::OnWeaponMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DrawMontage )
	{
		Begin_Equip();
	}
	else if (Montage == SheathMontage)
	{
		End_UnEquip();
	}
}



bool AWeapon::IsSameTagWithTarget(AActor* other, const FName& tag)
{
	return Owner->ActorHasTag(tag) && other->ActorHasTag(tag);
}

void AWeapon::PlayAttackMontage()
{
	CheckNull(AttackMontage);
	auto Player = Cast<APlayerCharacter>(GetOwnerCharacter());
	if (!Player)
	{
		GetOwnerCharacter()->PlayAnimMontage(AttackMontage);
	}
	else
	{
		Player->PlayAttackMontage(EquipType);
	}
}

void AWeapon::WeaponApplyDamage(AActor* OtherActor, const FHitResult& SweepResult)
{
	//자기자신 제외
	IgnoreActors.AddUnique(Owner);

	//서로 같은 그룹이면 비교하지 않는다
	if (IsSameTagWithTarget(OtherActor, "Enemy")) return;
	if (IsSameTagWithTarget(OtherActor, "Player")) return;

	//물리 충격을 가함
	CreateField(GetActorLocation());
	FVector Start = GetActorLocation();
	FVector End = Start + GetOwner()->GetActorForwardVector() * 5.f;
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, 20.f,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, { this,GetOwner()}, EDrawDebugTrace::ForDuration, HitResults,
		true);

	if (!!OtherActor && !IgnoreActors.Contains(OtherActor))
	{
		//GetFinalDamage함수로 한번에 가져오도록 수정
		//WeaponInterface도 함께 수정
		float AdditionalDamage = 0.f;
		if (Owner->ActorHasTag("Player"))
			AdditionalDamage = Cast<APlayerCharacter>(Owner)->GetStrDamage();

		for (auto result : HitResults)
		{
			IHitInterface* other = Cast<IHitInterface>(result.GetActor());
			if (other == nullptr || IgnoreActors.Contains(OtherActor)) continue;
			IgnoreActors.AddUnique(result.GetActor());
			result.ImpactPoint = GetActorLocation();
			float DamageValue = UGameplayStatics::ApplyPointDamage(OtherActor, Damage + AdditionalDamage, Owner->GetActorForwardVector(), result, WeaponInstigator, this, DamageTypeClass);
			if (DamageValue > 0.f)
				other->Hit(GetActorLocation());
		}
		
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
	Super::Equip(Type);

	SetInstigator(GetOwnerCharacter()->GetController()); //무기 변경 시 컨틀롤러 재등록 고려

	if (DrawMontage)
	{
		auto AnimInstance = GetOwnerCharacter()->GetMesh()->GetAnimInstance();
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

void AWeapon::UnEquip(EEquipType Type)
{
	Super::UnEquip(Type);

	if (SheathMontage)
	{
		auto AnimInstance = GetOwnerCharacter()->GetMesh()->GetAnimInstance();
		FString SectionName;

		if (Type == EEquipType::ET_LeftWeapon)
			SectionName = "Left";
		if (Type == EEquipType::ET_RightWeapon)
			SectionName = "Right";

		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName(SectionName));
	}
	else
		End_UnEquip();
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
	auto Player = Cast<APlayerCharacter>(GetOwnerCharacter());
	if (Player)
	{
		CheckFalse(Player->CanBlock());
		Player->SetBlock(true);
		Player->SetBlockStaminaRegenRate();
	}
}

void AWeapon::StrongAttack()
{
	CLog::Log(GetName() + " Strong Attack");
}

void AWeapon::SpecialAttack_Implementation()
{
	CLog::Log(GetName() + "This is Special Attack");
}

void AWeapon::Hit(const FVector& ImpackPoint)
{
	ASoundManager::GetSoundManager()->PlaySFXAtLocation(this, ESFXType::ESFXType_Guard, ImpackPoint);

	if (NSParticleComponent->GetAsset())
	{
		NSParticleComponent->Activate(true);
	}
}
