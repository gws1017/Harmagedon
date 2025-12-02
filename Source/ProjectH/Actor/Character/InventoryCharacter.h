#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/PlayerCharacter.h"
#include "InventoryCharacter.generated.h"


class USpotLightComponent;
class USceneCaptureComponent2D;

UCLASS()
class PROJECTH_API AInventoryCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AInventoryCharacter();

public:

	virtual void BeginPlay() override;

public:

	void SetCapture(AActor* InActor, const bool bIncludeFromChildActors);
	void RemoveCapture(AActor* InActor, const bool bIncludeFromChildActors);


private:

	UPROPERTY(EditDefaultsOnly, Category = "Ref")
	APlayerCharacter* PlayerRef;


	UPROPERTY(EditDefaultsOnly, Category = "Light")
	USpotLightComponent* SpotLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCapture;

};
