// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API ASword : public AWeapon
{
	GENERATED_BODY()
	
public:

	ASword();

public:

		virtual void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

		virtual void Equip(EEquipType Type) override;
		virtual void UnEquip(EEquipType Type) override;

		virtual void BasicAttack() override;
};
