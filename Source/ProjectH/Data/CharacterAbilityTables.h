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
		float TotalHP; //ü�¿� ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalStamina; //��¿� ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalMana; //������ ���� ����

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float PhyDmg;//�ٷ¿� �������� (�������ݷ�)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float MagDmg;//�žӿ� �������� (�������ݷ�)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 BleedResistance;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 PoisonResistance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 MaxMagicSlot; //���� ����, ������ ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float MaxWeight; //�ִ�����, ü�¿� ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float Poise; //���ε�, �ٷ¿� ���� ���� 

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 LevelUpExp;

};

UCLASS()
class PROJECTH_API ACharacterAbilityTables : public AActor
{
	GENERATED_BODY()

};
