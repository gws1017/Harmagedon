#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

struct FItemData;

enum class EItemType : uint8;
class AItem;
class USlot;


USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

public:

	FInventoryItem() = default;
	FInventoryItem(const FItemData& ItemData, bool bEquipped)
		: ItemInfo(ItemData), Count(1), bEquipped(bEquipped)
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		FItemData ItemInfo; //아이템정보만 저장되있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		int32 Count; //아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		bool bEquipped;  //장착여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		USlot* Slot;
	bool operator==(const FInventoryItem& OtherData) const
	{
		return this->ItemInfo.ItemCode == OtherData.ItemInfo.ItemCode;
	}
};

USTRUCT(BlueprintType)
struct FQuickSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventoryData")
	TArray<USlot*> ItemSlots;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventoryData")
		int32 SelectSlot = 0;//HUD에 노출되는, 현재 선택한 슬롯,UI로 옮길 수도있음
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	
	//Getter
	FORCEINLINE float GetCurrentWeight() const { return CurrentWeight; }
	FORCEINLINE FInventoryItem GetItemData(const int64 ItemCode) const { return InventoryContents.FindRef(ItemCode); }
	FInventoryItem GetItemDataFromSlot(USlot* Slot);
	//FORCEINLINE int32 GetSelectSlot(EEquipType Type) const { return EquipInfo[Type].SelectSlot; }
	
	AItem* GetCDOItem(TSubclassOf<AItem> Item) const;
	//인벤토리내 아이템 추가되거나 제거시 브로드캐스트할 것
	//용도는 아이템 업데이트시 UI가자동업데이트되기위함(UI호출하지않고 이벤트로 알아서처리)
	FOnInventoryUpdated OnInventoryUpdated;

	//인벤토리에 존재하는 아이템을 전부 돌려주는 함수
	TArray<FInventoryItem> GetInventoryContents() const;
	TArray<FInventoryItem> GetInventoryItemsFromType(const EItemType Type) const;

	//아이템 습득 및 삭제시 사용되는 함수(인벤토리에 추가 / 삭제)
	//플레이어 상호작용 및 UI에서 호출된다.
		void AddItem(const FItemData& ItemData, bool bEquipped = false);
	UFUNCTION(BlueprintCallable)
		void AddItem(const int64 ItemCode, bool bEquipped = false);
	UFUNCTION(BlueprintCallable)
		void RemoveItem(const int64 ItemCode);
	void ClearInventory();

	UFUNCTION(BlueprintCallable)
		void UpdateSlot();
	//아이템 장착시 사용되는 함수
	//void UpdateEquipItem(); //장착중인 아이템 확인후 플레이어 소켓에 부착한다.
	//UI에서 마우스입력을 받아 호출한다.
	//UFUNCTION(BlueprintCallable)
	//	void EquipInSlot(const int64 ItemCode , int32 SlotNumber); //인벤토리에 있는 아이템중에서, 특정 아이템을 장착상태로 전환한다.
	//UFUNCTION(BlueprintCallable)
	//	void UnEquipInSlot(EEquipType Type,int32 SlotNumber); //인벤토리에 있는 아이템중에서, 특정 아이템을 탈착상태로 전환한다.
	
	//실제 객체를 생성해서 탈부착하는 함수, Equip/UnEquip함수하나로 합칠것
	void Equip(USlot* Slot, AItem* ItemInstance = nullptr);
	void UnEquip(USlot* Slot);
	
	UFUNCTION(BlueprintCallable)
		void Use(const int64 ItemCode);

private:

	//아이템 추가시 최대 보유 수량을 넘지않는지 확인하는 함수
	bool CheckWeight(float weight);
protected:
	// 플레이어 인벤토리, 장비하지않은 아이템 포함
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
		TMap<int64, FInventoryItem> InventoryContents;
	////퀵슬롯에 설정한 정보를 담고 있음
	//UPROPERTY(VisibleAnywhere, Category = "Inventory")
	//	TMap<EEquipType, FQuickSlot> EquipInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventoryData")
		TArray<USlot*> ItemSlots;
	UPROPERTY(EditAnywhere, Category = "InventoryData", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<USlot> SlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		float Capacity;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
		float CurrentWeight;
};
