// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossHUDWidget.h"
#include "UI/BossHpBarWidget.h"
#include "Actor/Character/Cain.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Data/HCollision.h"

UBossHUDWidget::UBossHUDWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UBossHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UBossHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBossHUDWidget::InitBar(AActor* EnemyPawn)
{
	// 이름으로 UI 가져오기
	HpBar = Cast<UBossHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	ACain* bossPawn = Cast<ACain>(EnemyPawn);
	if (bossPawn)
	{
		HpBar->SetMaxHp(bossPawn->GetMaxHP());
		bossPawn->SetupHUDWidget(this);
	}
}
