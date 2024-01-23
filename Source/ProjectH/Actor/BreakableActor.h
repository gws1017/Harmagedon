#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class PROJECTH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

protected:
	virtual void BeginPlay() override;

public:	
	
	virtual void Hit(const FVector& ParticleSpawnLocation) override;

private:

	UPROPERTY(VisibleDefaultsOnly)
		UGeometryCollectionComponent* GeometryCollectionComponent;

};
