#include "Actor/Character/Enemy.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/EnemyController.h"
#include "Global.h"

#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Enemy.h"

AEnemy::AEnemy()
	: MaxHP(1), HP(1), Exp(1),
	AttackRange(20.f)
	, ActionState(EMonsterAction::EMA_Normal)
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<USphereComponent>(this, &AgroSphere, "AgroSphere", GetRootComponent());
	UHelpers::CreateComponent<USphereComponent>(this, &ActionSphere, "ActionSphere", GetRootComponent());

	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = nullptr;
	Controller = nullptr;
	Tags.Add("Enemy");
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//블루프린트 클래스를 등록할거면 생성자말고 BeginPlay에서해주어야한다
	AIControllerClass = EnemyControllerClass;
	//블루프린트 클래스로 등록된 객체로 교체하는 함수
	SpawnDefaultController();
	EnemyController = GetController<AEnemyController>();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	ActionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::ActionSphereOnOverlapBegin);
	ActionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::ActionSphereOnOverlapEnd);

	AgroSphere->InitSphereRadius(DetectRadius);
	ActionSphere->InitSphereRadius(ActionRadius);
	AgroSphere->SetRelativeLocation(FVector(DetectRadius, 0.f, 0.f));
	ActionSphere->SetRelativeLocation(FVector(ActionRadius, 0.f, 0.f));

	SpawnLocation = GetActorLocation();
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	DecrementEnemyFunc.Unbind();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageAmount <= 0.f)
		return DamageAmount;

	if (HP - DamageAmount <= 0.f) //체력이 0이될때 적용후 Die함수 호출
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
		CombatTarget->IncrementExp(Exp);
		Die();
	}
	else //일반적인 데미지 계산
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
	}
	UE_LOG(LogTemp, Display, L"Enemy Current HP : %f", HP);
	return DamageAmount;
}

void AEnemy::TargetApplyDamage(APlayerCharacter* player, float damage, const FVector& HitLocation)
{
	CheckNull(player);
	player->Hit(HitLocation);
	UGameplayStatics::ApplyDamage(player, damage, GetController(), this, TSubclassOf<UDamageType>());
}

FVector AEnemy::GetCombatTargetLocation() const
{
	CheckNullResult(CombatTarget, GetActorForwardVector());
	return CombatTarget->GetActorLocation();
}

void AEnemy::Begin_Attack()
{
}

void AEnemy::End_Attack()
{
	//Notify로 호출
	bAttacking = false;
}

void AEnemy::Attack()
{
	CheckFalse(Alive());
	CheckNull(CombatTarget);
	CheckFalse(CombatTarget->Alive());
	CheckFalse(bAlerted);
	CheckTrue(bAttacking);
	CheckTrue(ActionState == EMonsterAction::EMA_Stun);

	bAttacking = true;

	if (EnemyController)
	{
		GetController()->StopMovement();
	}
}

void AEnemy::Hit(const FVector& ParticleSpawnLocation)
{
	//AudioComponent->Play();
	if (HitParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ParticleSpawnLocation, FRotator(0.f), false);
	PlayAnimMontage(HitMontage);
}

void AEnemy::Stun()
{
	CheckNull(StunMontage);
	bAttacking = false;

	if (EnemyController)
	{
		GetController()->StopMovement();
		StopAnimMontage();
	}
	SetActionState(EMonsterAction::EMA_Stun);
	PlayAnimMontage(StunMontage);
	CLog::Print("Stun");
}

bool AEnemy::Alive()
{
	if (FMath::IsNearlyZero(HP)) return false;
	else return true;
}

void AEnemy::Die()
{
	if (DecrementEnemyFunc.IsBound())
		DecrementEnemyFunc.Execute();
	if (CombatTarget)
		CombatTarget = nullptr;

	StopAnimMontage();
	PlayAnimMontage(DeathMontage);

	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

void AEnemy::Disappear()
{
	//사라지면서 해야할 것들 작성
	Destroy();
}

void AEnemy::StopMove()
{
	StopAnimMontage();
	bAttacking = false;
}

bool AEnemy::IsHitActorAreaAttack(const FVector& start, const FVector& end, float radius, TArray<AActor*>& HitActors)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	EObjectTypeQuery Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);

	TArray<AActor*> IgnoreActors;
	//자기자신은 충돌검사 X
	IgnoreActors.Add(this);

	TArray<FHitResult> HitResults;
	bool result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, radius,
		ObjectTypes, false, IgnoreActors, bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, true);


	CheckFalseResult(result, result);

	for (auto hitresult : HitResults)
		HitActors.AddUnique(hitresult.GetActor());

	return result;
}

bool AEnemy::IsRanged(float radius)
{
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * radius*2;

	TArray<AActor*> HitActors;
	DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::White, true, 3.f, 0, 1.f);

	if (IsHitActorAreaAttack(start, end, radius, HitActors))
		return true;

	return false;
}


