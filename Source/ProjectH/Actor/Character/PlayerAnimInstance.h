#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
enum class EWeaponEquipped : uint8;
enum class EMovementState : uint8;

UCLASS()
class PROJECTH_API UPlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	/*UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		bool bIsRolling;*/

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		EWeaponEquipped WeaponEquipped;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		EMovementState MovementState;

private:
	UPROPERTY()
		APlayerCharacter* PlayerCharacter;
};
