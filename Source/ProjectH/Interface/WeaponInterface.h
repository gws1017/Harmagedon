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

//���⸦ ����ϴ� ���Ϳ��� ��ӵǴ� �������̽�
class PROJECTH_API IWeaponInterface
{
	GENERATED_BODY()

	//���������� �Ű������� ���� ���� ������ return�Ѵ�.
public:

	virtual AWeapon* GetWeapon(const EEquipType Type = EEquipType::ET_RightWeapon) const = 0;
	virtual float GetDamage(const EEquipType Type = EEquipType::ET_RightWeapon) const = 0;
};
