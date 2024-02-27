#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI/Slot.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
enum class EEquipType : uint8;
class AWeapon;

//무기를 사용하는 액터에게 상속되는 인터페이스
class PROJECTH_API IWeaponInterface
{
	GENERATED_BODY()

	//장착유형을 매개변수로 무기 관련 정보를 return한다.
public:

	virtual AWeapon* GetWeapon(const EEquipType Type = EEquipType::ET_RightWeapon) const = 0;
	virtual float GetDamage(const EEquipType Type = EEquipType::ET_RightWeapon) const = 0;
};
