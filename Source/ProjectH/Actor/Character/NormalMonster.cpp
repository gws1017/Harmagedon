#include "Actor/Character/NormalMonster.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/EnemyController.h"

#include "System/Sound/SoundManager.h"

#include "Global.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANormalMonster::ANormalMonster()
	:MaxStamina(50.f), StaminaRgenRate(2.0f)
	, ActionState(EMonsterAction::EMA_Normal)
{
	PrimaryActorTick.bCanEverTick = true;

	UHelpers::CreateComponent<UArrowComponent>(this, &SpawnDirection, "SpawnDirection", GetRootComponent());
	UHelpers::CreateComponent<USceneComponent>(this, &PatrolPositionComponent, "PatrolPositionComponent", GetRootComponent());

	CurrentStamina = MaxStamina;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();

	//기본 지정행동을 결정한다
	bPatrol = (GetPatrolPosition().Equals(GetActorLocation())) ? false : true;
	//Spawn 방향 Set
	if (!bPatrol)
	{
		FVector Dir = SpawnDirection->GetForwardVector();
		SetActorRotation(UKismetMathLibrary::MakeRotFromX(Dir));
	}
	else
	{
		PatrolPos = PatrolPositionComponent->GetComponentLocation();
	}

}

void ANormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStamina(DeltaTime);

	//락온
	if (bTargetLock)
	{
		if(CombatTarget)
			LastTargetPos = CombatTarget->GetActorLocation();
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LastTargetPos);
		SetActorRotation(rot);

		/*if (EnemyController)
			EnemyController->SetFocus(CombatTarget);*/
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
			SetActionState(EMonsterAction::EMA_Alert);
			CombatTarget = player;
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
			SetAlerted(false);
			CombatTarget = nullptr;
			SetActionState(EMonsterAction::EMA_Normal);
		}
	}
}

void ANormalMonster::ActionSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			SetActionState(EMonsterAction::EMA_AttackReady);
		}
	}
}

void ANormalMonster::ActionSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!!player)
		{
			SetActionState(EMonsterAction::EMA_Alert);
		}
	}
}

bool ANormalMonster::CanAttack() const
{
	bool result = true;

	CheckTrueResult(bAttacking, false);
	CheckTrueResult(ActionState == EMonsterAction::EMA_Stun, false);
	return result;
}

void ANormalMonster::Hit(const FVector& ParticleSpawnLocation)
{
	CheckFalse(CanHit());

	PlayHitEffect(ParticleSpawnLocation);

	if(EMonsterAction::EMA_Stun != ActionState)
		PlayAnimMontage(HitMontage);
}

void ANormalMonster::Stun()
{
	Super::Stun();
	SetActionState(EMonsterAction::EMA_Stun);
}

void ANormalMonster::SetChase(const float Speed)
{
	if (Speed == 400.f)
		bChased = true;
	else if (Speed == 200.f)
		bChased = false;

	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ANormalMonster::UpdateStamina(float DeltaTime)
{
	float DeltaStamina = StaminaRgenRate * DeltaTime;
	if (bAttacking) return;

	if (bChased)
	{
		CurrentStamina -= DeltaStamina;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);

		return;
	}
	CurrentStamina += DeltaStamina;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
}
