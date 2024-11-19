#include "Actor/Item/ExpItem.h"
#include "Actor/Character/PlayerCharacter.h"
#include "System/Sound/SoundManager.h"

#include "Components/SphereComponent.h"

void AExpItem::InitializeExp(int32 Amount, const FVector& Location)
{
	Exp = Amount;
	FVector Loc = Location;
	Loc.Z += (OverlapSphere->Bounds.SphereRadius) + HeightOffset;
	InitializeSpawnLocation(Loc);
}

void AExpItem::OnInteraction()
{
	PlayerInstance->IncrementExp(Exp);
	Exp = 0;
	ASoundManager::GetSoundManager()->PlaySFXAtLocation(PlayerInstance, ESFXType::ESFXType_Restore, GetActorLocation());

	APickupItem::OnInteraction();
}

void AExpItem::Use()
{
	AExpItem::OnInteraction();
}
