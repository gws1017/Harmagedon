#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Item.h"
#include "EquipmentItem.generated.h"

class ACharacter;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class PROJECTH_API AEquipmentItem : public AItem
{
	GENERATED_BODY()
	
public:
	AEquipmentItem();

protected:
	virtual void BeginPlay() override;

public:
	//Getter
	FORCEINLINE float GetPhysicalDefense() { return ItemData->StatData.PhyscialDeffenseRate * 0.01f; }
	FORCEINLINE bool GetEquipped() { return bEquipped; }
	FORCEINLINE bool GetEquipping() { return bEquipping; }

public:

	virtual void Equip(EEquipType Type) override;
	virtual void UnEquip(EEquipType Type) override;

	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Begin_UnEquip();
	virtual void End_UnEquip();
protected:

	//ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component", meta = (AllowPriavteAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* DrawMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* SheathMontage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipped;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipping;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		ACharacter* OwnerCharacter;
};
