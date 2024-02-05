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
		FItemData ItemInfo; //아이템정보만 저장되있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		bool bEquipped;  //장착여부

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
	
	//인벤토리내 아이템 추가되거나 제거시 브로드캐스트할 것
	//용도는 아이템 업데이트시 UI가자동업데이트되기위함(UI호출하지않고 이벤트로 알아서처리)
	FOnInventoryUpdated OnInventoryUpdated;

	//인벤토리에 존재하는 아이템을 전부 돌려주는 함수
	//TArray<FInventoryItem> GetInventoryContents() const;

	//void AddItem(const FItemData& ItemData, bool bEquipped = false);
	//void ClearInventory();

protected:
	// 플레이어 인벤토리
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
		TMap<int64, FInventoryItem> InventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
		TMap<EEquipType, AItem*> EquipInfo;

};
