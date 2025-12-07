#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/PlayerCharacter.h"
#include "InventoryCharacter.generated.h"


class USpotLightComponent;

UCLASS()
class PROJECTH_API AInventoryCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AInventoryCharacter();

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

//public:
//
//	void SetCapture(AActor* InActor, const bool bIncludeFromChildActors);
//	void RemoveCapture(AActor* InActor, const bool bIncludeFromChildActors);


private:

	UPROPERTY(EditDefaultsOnly, Category = "Ref")
	APlayerCharacter* PlayerRef;


	UPROPERTY(EditDefaultsOnly, Category = "Light")
	USpotLightComponent* SpotLight;


};
