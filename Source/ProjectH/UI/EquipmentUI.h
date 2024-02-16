#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "EquipmentUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UEquipmentUI : public UBaseUI
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
		int32 WidgetIndex = 0;
};