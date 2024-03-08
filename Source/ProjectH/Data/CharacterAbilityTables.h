#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CharacterAbilityTables.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAbilityData : public FTableRowBase
{
	GENERATED_BODY()
public:

	FCharacterAbilityData() : TotalHP(0.f),
		TotalStamina(0.f), TotalMana(0.f),
		LevelUpExp(0) {}

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalHP; //체력에 의해 증가
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalStamina; //기력에 의해 증가
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalMana; //지성에 의해 증가

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float PhyDmg;//근력에 의해증가 (물리공격력)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float MagDmg;//신앙에 의해증가 (마법공격력)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 BleedResistance;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 PoisonResistance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 MaxMagicSlot; //마법 슬롯, 지성에 의해 증가
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float MaxWeight; //최대하중, 체력에 의해 증가
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float Poise; //강인도, 근력에 의해 증가 

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 LevelUpExp;

};

UCLASS()
class PROJECTH_API ACharacterAbilityTables : public AActor
{
	GENERATED_BODY()

};
