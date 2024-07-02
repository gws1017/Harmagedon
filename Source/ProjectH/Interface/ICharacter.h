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

class PROJECTH_API IICharacter
{
	GENERATED_BODY()

	
public:

	//�÷��̾�, ���Ͱ� �������� ����ϴ� �������̽� Ŭ����

	virtual void DeathEnd() = 0;

	virtual bool Alive() = 0;

	virtual void Die() = 0;
};
