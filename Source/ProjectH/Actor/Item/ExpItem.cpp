#include "Actor/Item/ExpItem.h"
#include "Actor/Character/PlayerCharacter.h"

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
	
	APickupItem::OnInteraction();
}

void AExpItem::Use()
{
	AExpItem::OnInteraction();
}
