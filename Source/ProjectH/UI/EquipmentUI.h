#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "EquipmentUI.generated.h"

enum class EEquipType : uint8;
class USlot;

USTRUCT(BlueprintType)
struct FSlotArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
	TArray<USlot*> SlotArray;
};

UCLASS()
class PROJECTH_API UEquipmentUI : public UBaseUI
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquipSlot(USlot* InventorySlotInstance);
	UFUNCTION(BlueprintCallable)
	void AddEquipSlot(const EEquipType Type, USlot* EquipSlot);
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
	TMap<EEquipType, FSlotArray> SlotMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
		int32 WidgetIndex = 0;
};
