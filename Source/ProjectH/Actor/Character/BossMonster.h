// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/Enemy.h"
#include "BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API ABossMonster : public AEnemy
{
	GENERATED_BODY()
	
	
protected:

	virtual void BeginPlay() override;
};
