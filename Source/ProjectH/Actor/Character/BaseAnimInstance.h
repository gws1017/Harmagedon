// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

//class APlayerCharacter;

UCLASS()
class PROJECTH_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		float Direction;
	//UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		//EMovementState MovementState;

private:
	UPROPERTY()
		ACharacter* OwnerCharacter;
};
