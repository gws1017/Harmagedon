#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/NormalMonster.h"
#include "Interface/WeaponInterface.h"
#include "GuardianMeele.generated.h"

class AWeapon;

UCLASS()
class PROJECTH_API AGuardianMeele : public ANormalMonster, public IWeaponInterface
{
	GENERATED_BODY()
	
public:
	AGuardianMeele();

protected:
	virtual void BeginPlay() override;

public:
	//Getter
	virtual AWeapon* GetWeapon(const EEquipType Type) const override { return WeaponInstance; }
	float GetDamage(const EEquipType Type) const;

	virtual void Attack() override;

	virtual void Disappear() override;
	virtual void DecrementStamina() override;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* WeaponInstance;
};
