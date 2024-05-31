#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/EquipmentItem.h"
#include "Armor.generated.h"



UCLASS()
class PROJECTH_API AArmor : public AEquipmentItem
{
	GENERATED_BODY()

public:
	AArmor();

protected:
	virtual void BeginPlay() override;

protected:


};
