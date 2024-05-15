// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CainPatternInterface.generated.h"

DECLARE_DELEGATE(FCainMontageFinished);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCainPatternInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EMontages : uint8
{
	BATTLECRY = 0,
	PUNCH1,
	HOOK1,
	GRAB,
	THROWAWAY,
	SMASHHEAD,
	THROWROCK,
	STRONGKICK,
	DASH,
	BACKDASH,
	LEFTDASH,
	RIGHTDASH,
	PUNCH2,
	UPPERCUT,
	HOOK2,
	THROWDOWN,
	STOMP2,
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
	virtual void SetMontageFinDelegate(const FCainMontageFinished& InOnAttackFinished) = 0;
	virtual void PlayMontageByAI(EMontages AnimMon) = 0;

	virtual float GetAIDetectRoomRange() = 0;
	virtual bool IsHealthUnderHalf() = 0;
	virtual float GetAITurnSpeed() = 0;
};
