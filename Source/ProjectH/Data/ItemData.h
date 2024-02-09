#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class AItem;

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	ET_None UMETA(DisplayName = "None"),
	ET_Shoe UMETA(DisplayName = "Shoe"),
	ET_LeftHand UMETA(DisplayName = "LeftHand"),
	ET_RightHand UMETA(DisplayName = "RightHand"),
	ET_Top UMETA(DisplayName = "Top"),
	ET_Bottom UMETA(DisplayName = "Bottom"),
	ET_Head UMETA(DisplayName = "Head"),
	ET_Consumalbe UMETA(DisplayName = "Consumalbe"),
	ET_DefaultMax UMETA(DisplayName = "DefaultMax")
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "������ �̸�"))
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "������ ���� ���콺 �����ٴ�� ���� ����"))
		FText Description;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "���Ž� �ʿ� ��ȭ��"))
		float PurchasePrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "����"))
		float Weight;
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "������ ������(�κ��丮UI)"))
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "����� �������Ʈ Ŭ����"))
		TSubclassOf<AItem> ItemClass;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData() :
		ItemCode(-1),
		TextData(),
		NumericData(),
		AssetData()
	{

	}

	FItemData(int itemCode, FText name, FText description, float Price, float Weight)
		: ItemCode(itemCode), EquipType(EEquipType::ET_None)
	{
		TextData.Name = name;
		TextData.Description = description;

		NumericData.PurchasePrice = Price;
		NumericData.Weight = Weight;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int64 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemTextData TextData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemAssetData AssetData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		EEquipType EquipType;

	bool operator==(const FItemData& OtherData) const
	{
		return this->ItemCode == OtherData.ItemCode;
	}
};