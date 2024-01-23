#include "Actor/BreakableActor.h"
#include "Global.h"

#include "GeometryCollection/GeometryCollectionComponent.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<UGeometryCollectionComponent>(this, &GeometryCollectionComponent, "GeoMetryCollectionComponent");

	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::Hit(const FVector& ParticleSpawnLocation)
{
	//AudioÀç»ý
}


