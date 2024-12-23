#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "NormalMonster.generated.h"

class UArrowComponent;
class USceneComponent;

UENUM(BlueprintType)
enum class EMonsterAction : uint8
{
	EMA_None,
	EMA_Normal UMETA(DisplayName = "Normal"),
	EMA_Alert UMETA(DisplayName = "Alert"),
	EMA_AttackReady UMETA(DisplayName = "AttackReady"),
	EMA_StandBy UMETA(DisplayName = "StandBy"),
	EMA_Stun UMETA(DisplayName = "Stun"),
	EMA_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PROJECTH_API ANormalMonster : public AEnemy
{
	GENERATED_BODY()
	
public:

	ANormalMonster();

protected:

	virtual void BeginPlay() override;
public:

	virtual void Tick(float DeltaTime) override;

	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void ActionSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void ActionSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	//Getter
	FORCEINLINE EMonsterAction GetActionState() const { return ActionState; }
	FORCEINLINE FVector GetPatrolPosition() const { return PatrolPos; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetStamina() const { return CurrentStamina; }
	FORCEINLINE bool IsPatrol() const { return bPatrol; }
	FORCEINLINE bool IsLockOn() const { return bTargetLock; }
	FORCEINLINE bool CanAttack() const;


	//Setter
	FORCEINLINE void SetLockOn(const bool value) { bTargetLock = value; }
	FORCEINLINE void SetActionState(const EMonsterAction value) { ActionState = value; }

public:

	virtual void Hit(const FVector& ParticleSpawnLocation)override;
	virtual void Stun() override;
	void SetChase(const float Speed);

	virtual void DecrementStamina() {};

protected:

	void UpdateStamina(float DeltaTime);

protected:

	//스폰시 바라보는 방향을 결정
	UPROPERTY(EditAnywhere, Category = "Components")
		UArrowComponent* SpawnDirection;
	//배회하는 몬스터의 경우 위치를 지정해야함, 미지정시 Idle이 기본
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta =(MakeEditWidget))
		USceneComponent* PatrolPositionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
		FVector PatrolPos;
	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
		FVector LastTargetPos;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float CurrentStamina;
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float StaminaRgenRate;

	//false면 기본 행동 IDLE true면 기본행동 Patrol
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
		bool bPatrol = false;
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
		bool bTargetLock = false;
	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
		bool bChased = false;

	//몬스터의 행동패턴을 결정짓는 enum class
	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
		EMonsterAction ActionState;
};
