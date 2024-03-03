#include "Actor/Item/PickupItem.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

APickupItem::APickupItem()
{
	UHelpers::CreateComponent<USphereComponent>(this, &OverlapSphere, "OverlapSphere", GetRootComponent());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", OverlapSphere);
	UHelpers::CreateComponent<UNiagaraComponent>(this, &ItemEffect, "ItemEffect", OverlapSphere);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OverlapSphereBeginOverlap);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OverlapSphereEndOverlap);
}

void APickupItem::OverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* OverlapPlayer = Cast<APlayerCharacter>(OtherActor);

	if (OverlapPlayer)
	{
		PlayerInstance = OverlapPlayer;
		PlayerInstance->SetOverlappingItem(this);
		//UI띄울거면 여기에
	}
}

void APickupItem::OverlapSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* OverlapPlayer = Cast<APlayerCharacter>(OtherActor);

	if (OverlapPlayer)
	{
		CheckNull(PlayerInstance);
		PlayerInstance->SetOverlappingItem(nullptr);
		PlayerInstance = nullptr;
	}
}

void APickupItem::OnInteraction()
{
	if (ItemEffect)
		ItemEffect->Deactivate();
	Destroy();
}
