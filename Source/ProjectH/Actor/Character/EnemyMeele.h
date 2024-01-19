// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "EnemyMeele.generated.h"

class AWeapon;

UCLASS()
class PROJECTH_API AEnemyMeele : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyMeele();

protected:
	virtual void BeginPlay() override;

public:
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	//Getter
	FORCEINLINE virtual AWeapon* GetWeapon() const override { return WeaponInstance; }

	virtual void Attack() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* WeaponInstance;
};
