// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BossGimmick.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Actor/Character/Cain.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Utilities/UHelpers.h"

// Sets default values
ABossGimmick::ABossGimmick()
{
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    // Set the size of the trigger box
    TriggerBox->InitBoxExtent(FVector(50.f, 50.f, 50.f));
    TriggerBox->SetCollisionProfileName("Trigger");

    // Bind the overlap events to the respective functions
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossGimmick::OnOverlapBegin);
}

void ABossGimmick::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
    if (playerActor && playerActor->GetCapsuleComponent() == OtherComp)
    {
        bPlayerEnterRoom = true;

        ABasicPlayerController* PlayerController = Cast<ABasicPlayerController>(playerActor->GetController());
        if (PlayerController)
        {
            TArray<ACain*> cain;
            UHelpers::FindActors(GetWorld(), cain);
            PlayerController->ShowBossHUD(cain[0]);
        }
    }
}

