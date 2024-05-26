#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class AItem;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_None UMETA(DisplayName = "None"),
	IT_Cousumable UMETA(DisplayName = "Consumable"),
	IT_Upgrade UMETA(DisplayName = "Upgrade"),
	IT_Key UMETA(DisplayName = "Key"),
	IT_Quest UMETA(DisplayName = "Quest"),
	IT_PhysicalWeapon UMETA(DisplayName = "PhysicalWeapon"),
	IT_MagicalWeapon UMETA(DisplayName = "MagicalWeapon"),
	IT_Shield UMETA(DisplayName = "Shield"),
	IT_Head UMETA(DisplayName = "Head"),
	IT_Top UMETA(DisplayName = "Top"),
	IT_Bottom UMETA(DisplayName = "Bottom"),
	IT_Shoe UMETA(DisplayName = "Shoe"),
	IT_Hand UMETA(DisplayName = "Hand"),
	IT_DefaultMax UMETA(DisplayName = "DefaultMax")
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "아이템 이름"))
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true,ToolTip = "아이템 설명 마우스 가져다대면 띄우기 가능"))
		FText Description;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "구매시 필요 재화량"))
		float PurchasePrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "무게"))
		float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "최대 겹침수"))
		int32 MaxStackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "겹침 여부"))
		bool bIsStackable;

};

USTRUCT(BlueprintType)
struct FItemStatData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "물리 공격력"))
		float PhysicalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "빛 공격력"))
		float LightDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "어둠 공격력"))
		float DarkDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "치명 공격력"))
		float CriticalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "강인도"))
		float Poise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "물리 커트율"))
		float PhyscialDeffenseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "어둠 커트율"))
		float DarkDeffenseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "빛 커트율"))
		float LightDeffenseRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "필요 근력"))
		float RequiuredStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "필요 체력"))
		float RequiureVitality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "필요 신앙"))
		float RequiuredFaith;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "필요 지성"))
		float RequiuredIntelligence;
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
		TextData(),
		NumericData(),
		StatData(),
		AssetData()
	{

	}

	FItemData(int itemCode, FText name, FText description, float Price, float Weight)
		: ItemCode(itemCode), ItemType(EItemType::IT_None)
	{
		TextData.Name = name;
		TextData.Description = description;

		NumericData.PurchasePrice = Price;
		NumericData.Weight = Weight;
		NumericData.bIsStackable = false;
		NumericData.MaxStackSize = 99;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int64 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemTextData TextData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemStatData StatData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FItemAssetData AssetData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		EItemType ItemType;

	bool operator==(const FItemData& OtherData) const
	{
		return this->ItemCode == OtherData.ItemCode;
	}
};