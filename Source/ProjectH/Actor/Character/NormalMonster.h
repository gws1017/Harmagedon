#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "NormalMonster.generated.h"

class UArrowComponent;

UENUM(BlueprintType)
enum class EMonsterAction : uint8
{
	EMA_None,
	EMA_Normal UMETA(DisplayName = "Normal"),
	EMA_Alert UMETA(DisplayName = "Alert"),
	EMA_AttackReady UMETA(DisplayName = "AttackReady"),
	EMA_StandBy UMETA(DisplayName = "StandBy"),
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

public:
	//Getter
	FORCEINLINE EMonsterAction GetActionState() const { return ActionState; }
	FORCEINLINE FVector GetPatrolPosition() const { return PatrolPosition; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetStamina() const { return CurrentStamina; }
	FORCEINLINE bool IsPatrol() const { return bPatrol; }

	//Setter
	FORCEINLINE void SetLockOn(const bool value) { bTargetLock = value; }
	FORCEINLINE void SetActionState(const EMonsterAction value ) { ActionState = value; }

	void SetChase(const float Speed);
	bool IsRanged();

private:

	void UpdateStamina(float DeltaTime);

protected:

	//스폰시 바라보는 방향을 결정
	UPROPERTY(EditAnywhere, Category = "Components")
		UArrowComponent* SpawnDirection;
	//배회하는 몬스터의 경우 위치를 지정해야함, 미지정시 Idle이 기본
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta =(MakeEditWidget))
		FVector PatrolPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float MaxStamina;
	UPROPERTY(EditDefaultsOnly, Category = "Status")
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
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
		EMonsterAction ActionState;
};
