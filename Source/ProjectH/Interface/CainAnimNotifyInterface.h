// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CainAnimNotifyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCainAnimNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTH_API ICainAnimNotifyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttackHitCheck() = 0;
	virtual void AttackHitCheck2() = 0;
	virtual void AttackHitCheckEnd() = 0;
};