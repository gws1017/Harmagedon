#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Item.h"
#include "EquipmentItem.generated.h"

class ACharacter;
class USceneComponent;

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
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetPhysicalDefenseRate() { return ItemData->StatData.PhyscialDeffenseRate * 0.01f; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetLightDefenseRate() { return ItemData->StatData.LightDeffenseRate * 0.01f; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetDarkDefenseRate() { return ItemData->StatData.DarkDeffenseRate * 0.01f; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetPoise() { return ItemData->StatData.Poise; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRequireStr() { return ItemData->StatData.RequiuredStrength; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRequireVit() { return ItemData->StatData.RequiureVitality; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRequireInt() { return ItemData->StatData.RequiuredIntelligence; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRequireFaith() { return ItemData->StatData.RequiuredFaith; }


	FORCEINLINE bool GetEquipped() { return bEquipped; }
	FORCEINLINE bool GetEquipping() { return bEquipping; }

	UFUNCTION(BlueprintCallable)
		ACharacter* GetOwnerCharacter();

public:

	virtual void Equip(EEquipType Type) override;
	virtual void UnEquip(EEquipType Type) override;

	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Begin_UnEquip();
	virtual void End_UnEquip();
protected:

	//������Ʈ
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USceneComponent* Scene;





	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		bool bEquipped;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		bool bEquipping;
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		ACharacter* OwnerCharacter;
};
