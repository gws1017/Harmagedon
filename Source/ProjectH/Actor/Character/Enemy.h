#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Interface/ICharacter.h"
#include "Enemy.generated.h"

class APlayerCharacter;
class AEnemyController;
class AController;
class UParticleSystem;
class USphereComponent;
class UAnimMontage;
class UDamageType;

DECLARE_DELEGATE(FDercementEnemyCount);

UCLASS()
class PROJECTH_API AEnemy : public ACharacter, public IICharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	
	AEnemy();

protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void TargetApplyDamage(APlayerCharacter* player, float damage, const FVector& HitLocation = FVector::ZeroVector);

	UFUNCTION()
		virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {};
	UFUNCTION()
		virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {};

	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {};
	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {};

public:

	//Getter
	FORCEINLINE bool GetAlerted() { return bAlerted; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE APlayerCharacter* GetTarget() { return (!!CombatTarget) ? CombatTarget : nullptr; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetCurrentHP() { return HP; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxHP() { return MaxHP; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetExp() { return Exp; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetAttackRange() { return AttackRange; }
	FVector GetCombatTargetLocation() const;

	//Setter
	//FORCEINLINE void SetSpawner(class AEnemySpawner* obj) { Spawner = obj; }
	FORCEINLINE void SetAlerted(bool value) { bAlerted = value; }

	
	
	FORCEINLINE void InitTarget() { CombatTarget = nullptr; }

	virtual void Begin_Attack();
	virtual void End_Attack();
	virtual void Attack();

	virtual void Hit(const FVector& ParticleSpawnLocation);

	virtual bool Alive();

	virtual void Die();
	virtual void DeathEnd();
	virtual void Disappear();

	void AlertEnd();

	UFUNCTION()
		virtual bool IsHitActorAreaAttack(const FVector& start, const FVector& end,
			float radius, TArray<AActor*>& HitActors);
	UFUNCTION(BlueprintPure)
		bool IsRanged(float radius );

	FDercementEnemyCount DecrementEnemyFunc;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Status")
		float MaxHP;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Status")
		float HP;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Exp;
	UPROPERTY(EditAnywhere, Category = "AI")
		float AttackRange;

	UPROPERTY(VisibleDefaultsOnly, Category = "Status")
		FVector SpawnLocation;

	//UPROPERTY(VisibleDefaultsOnly)
		//class AEnemySpawner* Spawner;

	
	UPROPERTY(EditAnywhere, Category = "AI")
		TSubclassOf<AEnemyController> EnemyControllerClass;
	UPROPERTY(EditAnywhere, Category = "AI")
		AEnemyController* EnemyController;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Component")
		USphereComponent* AgroSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		USphereComponent* CombatSphere;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
		bool bAlerted;
	UPROPERTY(EditAnywhere, Category = "AI")
		bool bShowDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* HitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		UParticleSystem* HitParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		APlayerCharacter* CombatTarget;


	FTimerHandle AlertTimer;
	float AlertDuration = 3.0f;
	FTimerHandle DeathTimer;
	float DeathDelay = 3.0f;

	bool bAttacking = false;
};
