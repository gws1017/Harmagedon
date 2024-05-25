// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "BossHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBossHUDWidget : public UBaseUI
{
	GENERATED_BODY()
	
public:
	UBossHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	//void UpdateStat(const FPdCharacterStat& BaseStat, const FPdCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

// 2개의 UI 소유
protected:
	UPROPERTY()
	TObjectPtr<class UBossHpBarWidget> HpBar;

	//UPROPERTY()
	//TObjectPtr<class UPdCharacterStatWidget> CharacterStat;
};
