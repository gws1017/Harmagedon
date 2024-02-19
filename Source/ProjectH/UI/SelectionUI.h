#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "SelectionUI.generated.h"

class USlot;

UCLASS()
class PROJECTH_API USelectionUI : public UBaseUI
{
	GENERATED_BODY()

public :

	UFUNCTION(BlueprintCallable)
		void OnThrow(USlot* SlotData);
	
};
