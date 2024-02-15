#pragma once


#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "Data/ItemData.h"
#include "Slot.generated.h"

DECLARE_DYNAMIC_DELEGATE(FSlotClick);

struct FInventoryItem;

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	ET_None UMETA(DisplayName = "None"),
	ET_Shoe UMETA(DisplayName = "Shoe"),
	ET_Hand UMETA(DisplayName = "Hand"),
	ET_LeftWeapon UMETA(DisplayName = "LeftWeapon"),
	ET_RightWeapon UMETA(DisplayName = "RightWeapon"),
	ET_Top UMETA(DisplayName = "Top"),
	ET_Bottom UMETA(DisplayName = "Bottom"),
	ET_Head UMETA(DisplayName = "Head"),
	ET_Consumalbe UMETA(DisplayName = "Consumalbe"),
	ET_DefaultMax UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class PROJECTH_API USlot : public UBaseUI
{
	GENERATED_BODY()
	
public:

	virtual void NativePreConstruct();

public:

	UFUNCTION(BlueprintCallable)
		void SetItemCode(const int64 Code) { ItemInfo.ItemCode = Code; }
	
	UFUNCTION(BlueprintCallable)
		void SlotUpdate();
	UFUNCTION(BlueprintCallable)
		void SlotUpdateFromData(const FInventoryItem Data);
	UFUNCTION(BlueprintCallable)
	virtual void OnClickSlot();

	UFUNCTION(BlueprintCallable)
		void AddClickFunction(UObject* InObject, const FName InFunctionName);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		FItemData ItemInfo; //아이템정보만 저장되있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		int32 Count; //아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		bool bEquipped;  //장착여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		EEquipType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
		FSlotClick SlotClickFunction;
};
