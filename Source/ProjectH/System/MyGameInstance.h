// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UMyGameInstance();

public:

	struct FCharacterAbilityData* GetCharAbilityData(int32 Level);


private:

	UPROPERTY(EditAnywhere, Category = "Data");
	class UDataTable* CharacterAbilityData;
};
