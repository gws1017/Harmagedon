#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Item.h"
#include "HealPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECTH_API AHealPotion : public AItem
{
	GENERATED_BODY()
	
public:

	AHealPotion();

public:

	virtual void Use() override;
	virtual void UseFail() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DrinkMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		UStaticMeshComponent* Mesh;
};
