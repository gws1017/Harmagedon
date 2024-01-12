#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Item.h"
#include "Weapon.generated.h"

class AController;
class ACharacter;
class UAnimMontage;
class USceneComponent;
class UBoxComponent;
class USkeletalMeshComponent;
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
		void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	template<typename T>
	static AWeapon* Spawn(class UWorld* InWorld, TSubclassOf<T> BPClass, ACharacter* InOwner)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		return InWorld->SpawnActor<T>(BPClass, params);
	}

public:

	//Getter
	FORCEINLINE bool GetEquipped() { return bEquipped; }
	FORCEINLINE bool GetEquipping() { return bEquipping; }

	FORCEINLINE float GetDamage() { return Damage; }
	FORCEINLINE float GetStaminaCost() { return StaminaCost; }

	FORCEINLINE class UBoxComponent* GetWeaponCollision() { return WeaponCollision; }

	//Setter
	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

public:

	void ActivateCollision();
	void DeactivateCollision();
	UFUNCTION()
		void CreateField(const FVector& FieldLocation);

	virtual void Equip();
	virtual void UnEquip();

	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Begin_UnEquip();
	virtual void End_UnEquip();

	virtual void Begin_Collision();
	virtual void End_Collision();

protected:

	bool IsSameTagWithTarget(AActor* other, const FName& tag);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UBoxComponent* WeaponCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* DrawMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* SheathMontage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Socket")
		FName DrawSocket = "DrawSocket";
	UPROPERTY(VisibleDefaultsOnly, Category = "Socket")
		FName SheathSocket = "SheathSocket";

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float Damage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float AdditionalDamage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float StaminaCost;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipped;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipping;

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
