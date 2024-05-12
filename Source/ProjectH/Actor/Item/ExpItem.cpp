#include "Actor/Item/ExpItem.h"
#include "Actor/Character/PlayerCharacter.h"
#include "System/Sound/SoundManager.h"

#include "Components/SphereComponent.h"

void AExpItem::Init(int32 LostExp, const FVector& Location)
{
	Exp = LostExp;
	FVector Loc = Location;
	Loc.Z += (OverlapSphere->Bounds.SphereRadius) + HeightOffset;
	SetActorLocation(Loc);
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
