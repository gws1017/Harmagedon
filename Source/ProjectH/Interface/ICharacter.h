#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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

	FORCEINLINE virtual AWeapon* GetWeapon() const  = 0;
	virtual void DeathEnd() = 0;

};
