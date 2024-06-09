// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossGimmick.generated.h"

UCLASS()
class PROJECTH_API ABossGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossGimmick();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
    class UBoxComponent* TriggerBox;

    // Overlap begin function
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult & SweepResult);

    UFUNCTION()
    bool IsPlayerEnterRoom() const {return bPlayerEnterRoom;}

private:
    bool bPlayerEnterRoom;
};
