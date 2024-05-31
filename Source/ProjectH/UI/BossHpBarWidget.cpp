// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Actor/Character/Cain.h"

UBossHpBarWidget::UBossHpBarWidget(const FObjectInitializer& ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UBossHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 에디터에서 설정한 이름으로 위젯 가져오기
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BossHpBar")));
	ensure(HpProgressBar);

	// 캐릭터 위젯 설정
	ACain* CharacterWidget = Cast<ACain>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UBossHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
