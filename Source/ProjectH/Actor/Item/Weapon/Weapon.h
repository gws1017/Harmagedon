#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/EquipmentItem.h"
#include "Interface/HitInterface.h"
#include "Weapon.generated.h"


class AController;
class ACharacter;
class UAnimMontage;
class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UDamageType;
class UFieldSystemComponent;
class URadialFalloff;
class URadialVector;
class UFieldSystemMetaDataFilter;
class UNiagaraSystem;

UCLASS()
class PROJECTH_API AWeapon : public AEquipmentItem, public IHitInterface
{
	GENERATED_BODY()


public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		virtual void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {};

public:

	//Getter
	FORCEINLINE float GetPhysicalDamage() { return ItemData->StatData.PhysicalDamage; }
	FORCEINLINE float GetLightDamage() { return ItemData->StatData.LightDamage; }
	FORCEINLINE float GetDarkDamage() { return ItemData->StatData.DarkDamage; }

	FORCEINLINE float GetStaminaCost() { return StaminaCost; }

	FORCEINLINE class UBoxComponent* GetWeaponCollision() { return WeaponCollision; }

	FORCEINLINE UAnimMontage* GetAttackMontage() { return AttackMontage; }
	//Setter
	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

public:

	void ActivateCollision();
	void DeactivateCollision();
	UFUNCTION()
		void CreateField(const FVector& FieldLocation);

	virtual void Equip(EEquipType Type) override;
	virtual void UnEquip(EEquipType Type) override;

	virtual void Begin_Collision();
	virtual void End_Collision();

	UFUNCTION()
		virtual void BasicAttack();
	UFUNCTION()
		virtual void Block();
	UFUNCTION()
		virtual void StrongAttack();
	UFUNCTION(BlueprintNativeEvent)
		void SpecialAttack();
	virtual void SpecialAttack_Implementation();

	virtual void Hit(const FVector& ImpackPoint) override;

protected:

	bool IsSameTagWithTarget(AActor* other, const FName& tag);

	void PlayAttackMontage();

	void WeaponApplyDamage(AActor* OtherActor, const FHitResult& SweepResult);

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component", meta = (AllowPriavteAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UBoxComponent* WeaponCollision;

	//무기 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* DrawMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* SheathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* BlockMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* StrongAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* SpecialAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		UNiagaraSystem* HitParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "Socket")
		FName SheathSocket = "SheathSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Damage;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float StaminaCost;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float SpecialAttackStaminaCost;

	//물리
	UPROPERTY(VisibleDefaultsOnly, Category = "Field")
		UFieldSystemComponent* FieldSystemComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "Field")
		URadialFalloff* RadialFalloff;
	UPROPERTY(VisibleDefaultsOnly, Category = "Field")
		URadialVector* RadialVector;
	UPROPERTY(VisibleDefaultsOnly, Category = "Field")
		UFieldSystemMetaDataFilter* MetaData;
	UPROPERTY(EditAnywhere, Category = "Field")
		float RadialFalloffMagnitude;
	UPROPERTY(EditAnywhere, Category = "Field")
		float RadialVectorMagnitude;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		TArray<AActor*> IgnoreActors;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		AController* WeaponInstigator;

};
