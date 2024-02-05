#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

struct FItemData;

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		FItemData ItemInfo; //������������ ������ִ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		bool bEquipped;  //��������

	bool operator==(const FInventoryItem& OtherData) const
	{
		return this->ItemInfo.ItemCode == OtherData.ItemInfo.ItemCode;
	}
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
	
	//�κ��丮�� ������ �߰��ǰų� ���Ž� ��ε�ĳ��Ʈ�� ��
	//�뵵�� ������ ������Ʈ�� UI���ڵ�������Ʈ�Ǳ�����(UIȣ�������ʰ� �̺�Ʈ�� �˾Ƽ�ó��)
	FOnInventoryUpdated OnInventoryUpdated;

	//�κ��丮�� �����ϴ� �������� ���� �����ִ� �Լ�
	//TArray<FInventoryItem> GetInventoryContents() const;

	//void AddItem(const FItemData& ItemData, bool bEquipped = false);
	//void ClearInventory();

protected:
	// �÷��̾� �κ��丮
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
		TMap<int64, FInventoryItem> InventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
		TMap<EEquipType, AItem*> EquipInfo;

};
