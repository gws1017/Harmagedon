#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class AItem;

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	EEquipType_None UMETA(DisplayName = "None"),
	EEquipType_Shoe UMETA(DisplayName = "Shoe"),
	EEquipType_LeftHand UMETA(DisplayName = "LeftHand"),
	EEquipType_RightHand UMETA(DisplayName = "RightHand"),
	EEquipType_Top UMETA(DisplayName = "Top"),
	EEquipType_Bottom UMETA(DisplayName = "Bottom"),
	EEquipType_Head UMETA(DisplayName = "Head"),
	EEquipType_Consumalbe UMETA(DisplayName = "Consumalbe"),
	EEquipType_DefaultMax UMETA(DisplayName = "DefaultMax")
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "아이템 이름"))
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "아이템 설명 마우스 가져다대면 띄우기 가능"))
		FText Description;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "구매시 필요 재화량"))
		float PurchasePrice;
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "아이템 아이콘(인벤토리UI)"))
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "사용할 블루프린트 클래스"))
		TSubclassOf<AItem> ItemClass;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData() :
		ItemCode(-1),
		EquipType(),
		TextData(),
		NumericData(),
		AssetData()
	{

	}

	FItemData(int itemCode, EEquipType equipType, FText name, FText description, float numericData, int32 sellValue)
		: ItemCode(itemCode), EquipType(equipType)
	{
		TextData.Name = name;
		TextData.Description = description;

		NumericData.PurchasePrice = numericData;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int64 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")

		EEquipType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemTextData TextData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemAssetData AssetData;


	bool operator==(const FItemData& OtherData) const
	{
		return this->ItemCode == OtherData.ItemCode;
	}
};