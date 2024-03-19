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
	FORCEINLINE bool CanAttack() const;


	//Setter
	FORCEINLINE void SetLockOn(const bool value) { bTargetLock = value; }
	FORCEINLINE void SetActionState(const EMonsterAction value ) { ActionState = value; }

	void SetChase(const float Speed);

private:

	void UpdateStamina(float DeltaTime);

protected:

	//������ �ٶ󺸴� ������ ����
	UPROPERTY(EditAnywhere, Category = "Components")
		UArrowComponent* SpawnDirection;
	//��ȸ�ϴ� ������ ��� ��ġ�� �����ؾ���, �������� Idle�� �⺻
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

	//false�� �⺻ �ൿ IDLE true�� �⺻�ൿ Patrol
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
		bool bPatrol = false;
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
		bool bTargetLock = false;
	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
		bool bChased = false;
	//������ �ൿ������ �������� enum class
	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
		EMonsterAction ActionState;
};
