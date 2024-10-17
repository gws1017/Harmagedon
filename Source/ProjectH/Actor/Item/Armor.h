#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/EquipmentItem.h"
#include "Armor.generated.h"

class USkeletalMesh;
class UStaticMesh;

UCLASS()
class PROJECTH_API AArmor : public AEquipmentItem
{
	GENERATED_BODY()

public:
	AArmor();

protected:
	virtual void BeginPlay() override;

public:

	//Getter
	USkeletalMesh* GetSkeletalMesh() { return SkeletalMesh; }
	TArray<UStaticMesh*> GetStaticMeshArray() { return StaticMeshes; }
	virtual void Equip(EEquipType Type);
	//virtual void UnEquip(EEquipType Type);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Component", meta = (AllowPriavteAccess = "true"))
		USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Component", meta = (AllowPriavteAccess = "true"))
		TArray<UStaticMesh*> StaticMeshes;
};
