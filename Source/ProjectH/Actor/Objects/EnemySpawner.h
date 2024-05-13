// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"


class UBoxComponent;
class AEnemy;

UCLASS()
class PROJECTH_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemySpawner();

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void SpawnMonster();
	UFUNCTION()
		void RespawnMonster();
	UFUNCTION()
		void DecrementEnemyCount() { if (MonsterCount > 0)MonsterCount--; }
private:

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
		int32 MonsterCount = 0;

};
