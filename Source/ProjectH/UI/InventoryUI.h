#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "Data/ItemData.h"
#include "InventoryUI.generated.h"

struct FInventoryItem;
enum class EItemType : uint8;
enum class EEquipType : uint8;

USTRUCT(BlueprintType)
struct FTabData
{
	GENERATED_BODY()

public:
	FTabData() :
		ItemType(EItemType::IT_None)
	{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TabData")
		FText TabName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TabData")
		EItemType ItemType;
};

UCLASS()
class PROJECTH_API UInventoryUI : public UBaseUI
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable)
		TArray<FInventoryItem> GetInventoryItemsFromEquipType(const EEquipType Type);
	UFUNCTION(BlueprintCallable)
		TArray<FInventoryItem> GetInventoryItemsFromItemType(const EItemType Type);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
		TArray<int32> TabIndexArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
		TArray<FTabData> TabDataArray;
};
