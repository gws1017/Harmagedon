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

	// �̸����� UI ��������
	HpBar = Cast<UBossHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	// �� HUD ���� �� ��������
	APlayerCharacter* HUDPawn = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	// �� ��ġ�� ���� ���� ���� ��������
	FVector Center = HUDPawn->GetActorLocation();
	UWorld* World = HUDPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	// ��� �� �����ϱ�
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, HUDPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_HACTION,
		FCollisionShape::MakeSphere(3000.0f),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			ACain* EnemyPawn = Cast<ACain>(OverlapResult.GetActor());
			// ���� �߿� �츮 �÷��̾��� ���
			if (EnemyPawn)
			{
				HpBar->SetMaxHp(EnemyPawn->GetMaxHP());
				EnemyPawn->SetupHUDWidget(this);
			}
		}
	}
}
