#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Actor/Character/PlayerCharacter.h"
#include "MySaveGame.generated.h"

struct FPlayerStatus;

USTRUCT(BlueprintType)
struct FSaveData {
	GENERATED_BODY()
public:

	FSaveData() :
		LostExp(0),
		Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), StartPoint(FVector::ZeroVector),DeathLocation(FVector::ZeroVector),
		CainDie(false)
	{};
	FSaveData(FPlayerStatus status, TMap<EEquipType, int32> einfo,int32 exp,
		FVector loc, FRotator rot, FVector start, FVector deathLoc, bool cainDie) :
		Status(status), EquipmentInfo(einfo),
		LostExp(exp),
		Location(loc), Rotation(rot), StartPoint(start), DeathLocation(deathLoc),
		CainDie(cainDie)
	{
	};

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		FPlayerStatus Status;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		TMap<EEquipType, int32> EquipmentInfo;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 LostExp;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		FVector Location;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		FRotator Rotation;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		FVector StartPoint;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		FVector DeathLocation;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		bool CainDie;
};

UCLASS()
class PROJECTH_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMySaveGame();

	UPROPERTY(VisibleAnywhere, Category = "Basic")
		FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
		uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
		FSaveData SaveData;
};
