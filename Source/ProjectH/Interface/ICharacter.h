#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI/Slot.h"
#include "ICharacter.generated.h"


UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AWeapon;

class PROJECTH_API IICharacter
{
	GENERATED_BODY()

	
public:

	//�÷��̾�, ���Ͱ� �������� ����ϴ� �������̽� Ŭ����

	virtual AWeapon* GetWeapon(EEquipType Type = EEquipType::ET_RightWeapon) const  = 0;
	virtual void DeathEnd() = 0;

};
