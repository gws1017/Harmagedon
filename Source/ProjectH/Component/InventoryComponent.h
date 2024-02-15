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
		FItemData ItemInfo; //������������ ������ִ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		int32 Count; //������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		bool bEquipped;  //��������
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
		int32 SelectSlot = 0;//HUD�� ����Ǵ�, ���� ������ ����,UI�� �ű� ��������
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
	//�κ��丮�� ������ �߰��ǰų� ���Ž� ��ε�ĳ��Ʈ�� ��
	//�뵵�� ������ ������Ʈ�� UI���ڵ�������Ʈ�Ǳ�����(UIȣ�������ʰ� �̺�Ʈ�� �˾Ƽ�ó��)
	FOnInventoryUpdated OnInventoryUpdated;

	//�κ��丮�� �����ϴ� �������� ���� �����ִ� �Լ�
	TArray<FInventoryItem> GetInventoryContents() const;
	TArray<FInventoryItem> GetInventoryItemsFromType(const EItemType Type) const;

	//������ ���� �� ������ ���Ǵ� �Լ�(�κ��丮�� �߰� / ����)
	//�÷��̾� ��ȣ�ۿ� �� UI���� ȣ��ȴ�.
		void AddItem(const FItemData& ItemData, bool bEquipped = false);
	UFUNCTION(BlueprintCallable)
		void AddItem(const int64 ItemCode, bool bEquipped = false);
	UFUNCTION(BlueprintCallable)
		void RemoveItem(const int64 ItemCode);
	void ClearInventory();

	UFUNCTION(BlueprintCallable)
		void UpdateSlot();
	//������ ������ ���Ǵ� �Լ�
	//void UpdateEquipItem(); //�������� ������ Ȯ���� �÷��̾� ���Ͽ� �����Ѵ�.
	//UI���� ���콺�Է��� �޾� ȣ���Ѵ�.
	//UFUNCTION(BlueprintCallable)
	//	void EquipInSlot(const int64 ItemCode , int32 SlotNumber); //�κ��丮�� �ִ� �������߿���, Ư�� �������� �������·� ��ȯ�Ѵ�.
	//UFUNCTION(BlueprintCallable)
	//	void UnEquipInSlot(EEquipType Type,int32 SlotNumber); //�κ��丮�� �ִ� �������߿���, Ư�� �������� Ż�����·� ��ȯ�Ѵ�.
	
	//���� ��ü�� �����ؼ� Ż�����ϴ� �Լ�, Equip/UnEquip�Լ��ϳ��� ��ĥ��
	void Equip(USlot* Slot, AItem* ItemInstance = nullptr);
	void UnEquip(USlot* Slot);
	
	UFUNCTION(BlueprintCallable)
		void Use(const int64 ItemCode);

private:

	//������ �߰��� �ִ� ���� ������ �����ʴ��� Ȯ���ϴ� �Լ�
	bool CheckWeight(float weight);
protected:
	// �÷��̾� �κ��丮, ����������� ������ ����
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
		TMap<int64, FInventoryItem> InventoryContents;
	////�����Կ� ������ ������ ��� ����
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
