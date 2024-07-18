#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitleController.generated.h"

class UTitleUI;

UCLASS()
class PROJECTH_API ATitleController : public APlayerController
{
	GENERATED_BODY()

public:

	ATitleController();

public:

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		UTitleUI* TitleUIInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UTitleUI> TitleUIClass;


};
