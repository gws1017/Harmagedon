// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneTrigger.generated.h"

class ULevelSequence;
class UBoxComponent;
class USoundCue;
class UStaticMeshComponent;

class ACain;
class APlayerCharacter;

UCLASS()
class PROJECTH_API ASceneTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ASceneTrigger();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UFUNCTION()
		bool IsPlayerEnterRoom() const { return bPlayerEnterRoom; }

private:

	void PlayScene(APlayerCharacter* player);
	void FinishScene(APlayerCharacter* player);

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "Sequence")
		ULevelSequence* CutScene;

	UPROPERTY(EditDefaultsOnly, Category = "Actor")
		FVector SpawnPosition;
	UPROPERTY(EditDefaultsOnly, Category = "Actor")
		TSubclassOf<ACain> CainBP;

	FTimerHandle CutSceneTimer;

	bool bPlayerEnterRoom;
};
