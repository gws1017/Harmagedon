#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsTrigger.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class PROJECTH_API APhysicsTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	APhysicsTrigger();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:

	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UBoxComponent* TriggerBox;
};
