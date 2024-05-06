// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CainPatternInterface.generated.h"

DECLARE_DELEGATE(FBattleCryFinished);
DECLARE_DELEGATE(FStraightFinished);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCainPatternInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Dash = 0,
	Run,
	Walk
};

 /**************************************************************************************************
 * 보스(카인) 패턴 인터페이스
 *
 * @author	조현식
 * @date	2024-05-04
 **************************************************************************************************/
class PROJECTH_API ICainPatternInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetAIBattleCryDelegate(const FBattleCryFinished& InOnAttackFinished) = 0;
	virtual void SetAIStraightDelegate(const FBattleCryFinished& InOnAttackFinished) = 0;

	virtual void BattleCryByAI() = 0;
	virtual void StraightByAI() = 0;


	virtual float GetAIDetectRoomRange() = 0;
	virtual float GetAIFarDetectRange() = 0;
	virtual float GetAINearDetectRange() = 0;
	virtual bool IsRanged(float radius) = 0;
	virtual bool IsHealthUnderHalf() = 0;
	virtual void SetDashProperty() = 0;
};
