#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	virtual void Equip(EEquipType Type) {};
	virtual void UnEquip(EEquipType Type) {};
	virtual void Use() {};

protected:

	void SetSocketName(EEquipType Type);


protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Socket")
		FName EquipSocket = "";
};
