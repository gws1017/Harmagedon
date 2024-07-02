// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathTrigger.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTH_API ADeathTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeathTrigger();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:	

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UBoxComponent* TriggerBox;

};
