#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Item.h"
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
class PROJECTH_API AWeapon : public AItem
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
	FORCEINLINE bool GetEquipped() { return bEquipped; }
	FORCEINLINE bool GetEquipping() { return bEquipping; }

	FORCEINLINE float GetDamage() { return Damage; }
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

	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Begin_UnEquip();
	virtual void End_UnEquip();

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

protected:

	//컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USceneComponent* Scene;

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

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		EEquipType EquipType;

	UPROPERTY(VisibleDefaultsOnly, Category = "Socket")
		FName SheathSocket = "SheathSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Damage;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float StaminaCost;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float SpecialAttackStaminaCost;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipped;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipping;

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
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		ACharacter* OwnerCharacter;

};
