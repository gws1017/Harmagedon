// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CainPatternInterface.generated.h"

DECLARE_DELEGATE(FCainMontageFinished);

UENUM(BlueprintType)
enum class EPattern : uint8
{
	STRONGKICK = 0,
	SMASHHEAD,
	THROWROCK,
	STOMP1,
	HOOK1,
	PUNCH1,
	GRAB,
	THROWAWAY,
	BATTLECRY,
	UPPERCUT,
	PUNCH2,
	HOOK2,
	THROWDOWN,
	STOMP2,
	DASH,
	BACKDASH,
	LEFTDASH,
	GROGGY
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCainPatternInterface : public UInterface
{
	GENERATED_BODY()
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
	virtual void PlayMontageByAI(EPattern AnimMon) = 0;
	virtual void JumpMontageSection(FName SectionName, EPattern AnimMon) = 0;
	virtual void GroggyAnim() = 0;

	virtual float GetAIDetectRoomRange() = 0;
	virtual bool IsHealthUnderHalf() = 0;
	virtual float GetAITurnSpeed() = 0;
	virtual bool AllowNextPattern() = 0;
	virtual bool IsFirstPhase() = 0;
	virtual void ChangeIntoSecondPhase() = 0;
	virtual int32 GetStatus() = 0;
	virtual int32 GetHitCount() = 0;
	virtual void SetPrevRandomNumber(int32 number) = 0;
	virtual int32 GetPrevRandomNumber() = 0;
	virtual bool IsSuccessParry() = 0;
	virtual void IsSuccessParry(bool flag) = 0;
};
