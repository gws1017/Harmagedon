#include "Actor/Objects/PhysicsTrigger.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

APhysicsTrigger::APhysicsTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<UBoxComponent>(this, &TriggerBox, "TriggerBox");
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "PhysicsMesh", TriggerBox);

	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void APhysicsTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APhysicsTrigger::OnComponentBeginOverlap);

}

void APhysicsTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	CheckNull(player);
	CLog::Print(OtherComp->GetOwner()->GetName());
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);

}



