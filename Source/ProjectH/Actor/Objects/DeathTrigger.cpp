#include "Actor/Objects/DeathTrigger.h"
#include "Interface/ICharacter.h"
#include "Global.h"

#include "Components/BoxComponent.h"

ADeathTrigger::ADeathTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
	UHelpers::CreateComponent<UBoxComponent>(this, &TriggerBox, "TriggerBox");

	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

void ADeathTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADeathTrigger::OnComponentBeginOverlap);

}

void ADeathTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IICharacter* CharRef = Cast<IICharacter>(OtherActor);

	CheckNull(CharRef);

	CharRef->Die();

}

