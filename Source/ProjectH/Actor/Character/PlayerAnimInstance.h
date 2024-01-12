// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
enum class EWeaponEquipped : uint8;

UCLASS()
class PROJECTH_API UPlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		bool bIsRolling;
		UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		EWeaponEquipped WeaponEquipped;
private:
	UPROPERTY()
		APlayerCharacter* PlayerCharacter;
};
