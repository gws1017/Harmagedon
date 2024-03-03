#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class APlayerCharacter;

UCLASS()
class PROJECTH_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		APlayerCharacter* PlayerInstance;
};
