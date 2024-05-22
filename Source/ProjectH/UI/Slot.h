#pragma once


#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "Data/ItemData.h"
#include "Slot.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FSlotClick,USlot*,SlotInstance);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSlotHover,USlot*,SlotInstance);

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
	ET_Cousumable UMETA(DisplayName = "Consumalbe"),
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
		void SlotUpdateFromSlot(const USlot* Data);

	UFUNCTION(BlueprintCallable)
	virtual void OnClickSlot();
	UFUNCTION(BlueprintCallable)
		virtual void OnHoverSlot();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void InitializeSlotInfo();

	UFUNCTION(BlueprintCallable)
		void AddClickFunction(UObject* InObject, const FName InFunctionName);
	UFUNCTION(BlueprintCallable)
		void AddHoverFunction(UObject* InObject, const FName InFunctionName);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		FItemData ItemInfo; //아이템정보만 저장되있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		int32 Count; //아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		bool bEquipped;  //장착여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		bool bShowInfo;  //장착여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		EEquipType EquipType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		AItem* ItemInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
		FSlotClick SlotClickFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
		FSlotHover SlotHoverFunction;
};
