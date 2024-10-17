#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ItemData.h"
#include "Item.generated.h"

enum class EEquipType : uint8;


UCLASS()
class PROJECTH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	virtual void BeginPlay() override;


public:
	template<typename T>
	static T* Spawn(class UWorld* InWorld, TSubclassOf<T> BPClass, ACharacter* InOwner)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		return InWorld->SpawnActor<T>(BPClass, params);
	}

public:

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FItemData GetItemData() { return *ItemData; }

public:	

	virtual void Equip(EEquipType Type) {};
	virtual void UnEquip(EEquipType Type) {};
	virtual void Use() {};
	virtual void UseFail() {};

protected:

	void SetSocketName(EEquipType Type);

	UFUNCTION(BlueprintCallable)
	void InitializeItemData();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int64 ItemCode;
	UPROPERTY(EditDefaultsOnly, Category = "Socket")
		FName EquipSocket = "";

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		EEquipType EquipType;

	FItemData* ItemData;
};
