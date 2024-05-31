// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossWidgetComponent.h"
#include "BossHpBarWidget.h"

void UBossWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UBossHpBarWidget* BossHpBarWidget = Cast<UBossHpBarWidget>(GetWidget());
	if (BossHpBarWidget)
	{
		BossHpBarWidget->SetOwningActor(GetOwner());
	}
}
