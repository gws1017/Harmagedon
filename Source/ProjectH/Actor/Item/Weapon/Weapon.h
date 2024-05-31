#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/EquipmentItem.h"
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

UCLASS()
class PROJECTH_API AWeapon : public AEquipmentItem
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
	FORCEINLINE float GetDamage() { return ItemData->StatData.PhysicalDamage; }
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

protected:

	bool IsSameTagWithTarget(AActor* other, const FName& tag);

	void PlayAttackMontage();

	void WeaponApplyDamage(AActor* OtherActor);

protected:


	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UBoxComponent* WeaponCollision;

	//무기 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* BlockMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* StrongAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* SpecialAttackMontage;


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
