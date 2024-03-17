#include "Actor/Character/NormalMonster.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANormalMonster::ANormalMonster()
	:MaxStamina(50.f), StaminaRgenRate(2.0f), ActionState(EMonsterAction::EMA_Normal)
{
	UHelpers::CreateComponent<UArrowComponent>(this, &SpawnDirection, "SpawnDirection", GetRootComponent());

	CurrentStamina = MaxStamina;
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();

	//기본 지정행동을 결정한다
	bPatrol = (PatrolPosition.IsNearlyZero()) ? false : true;
	//Spawn 방향 Set
	if (!bPatrol)
	{
		FVector Dir = SpawnDirection->GetForwardVector();
		SetActorRotation(UKismetMathLibrary::MakeRotFromX(Dir));
	}

}

void ANormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStamina(DeltaTime);

	//락온
	if (bTargetLock)
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation());
		SetActorRotation(rot);
	}
	
}

void ANormalMonster::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			SetAlerted(true);
			//
			//WeaponInstance->Equip(EEquipType::ET_RightWeapon);
			CombatTarget = player;
			//player->SetTarget(this);
		}
	}
}

void ANormalMonster::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			CombatTarget = nullptr;
			//WeaponInstance->UnEquip(EEquipType::ET_RightWeapon);
			//player->SetTarget(nullptr);
		}
	}
}

void ANormalMonster::SetChase(const float Speed)
{
	if (Speed == 400.f)
		bChased = true;
	else if (Speed == 200.f)
		bChased = false;

	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

bool ANormalMonster::IsRanged()
{
	//SphereTrace를이용해 플레이어 사거리 내인지 탐지
	return true;
}

void ANormalMonster::UpdateStamina(float DeltaTime)
{
	float DeltaStamina = StaminaRgenRate * DeltaTime;

	if (bChased)
	{
		CurrentStamina -= DeltaStamina;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);

		return;
	}
	CurrentStamina += DeltaStamina;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
}
