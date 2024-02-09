#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "Data/ItemData.h"
#include "Slot.generated.h"

/**
 * 
 */
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
	virtual void OnClickSlot();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		FItemData ItemInfo; //아이템정보만 저장되있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		int32 Count; //아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryData")
		bool bEquipped;  //장착여부

};
