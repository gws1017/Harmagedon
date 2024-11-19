#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Item.h"
#include "Interface/InteractionInterface.h"
#include "PickupItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class APlayerCharacter;

UCLASS()
class PROJECTH_API APickupItem : public AItem, public IInteractionInterface
{
	GENERATED_BODY()
	
public:

	APickupItem();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void OnInteraction();

	virtual void ToggleOverlapUI(ABasicPlayerController* PlayerController);

	void InitializeSpawnLocation(const FVector& Location);


protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USphereComponent* OverlapSphere;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	FText OverlapText;
	UPROPERTY(EditAnywhere)
	FVector SearchExtent;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UNiagaraComponent* ItemEffect;

	UPROPERTY(VisibleDefaultsOnly)
		APlayerCharacter* PlayerInstance;

};
