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

	// 이름으로 UI 가져오기
	HpBar = Cast<UBossHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	// 이 HUD 가진 폰 가져오기
	APlayerCharacter* HUDPawn = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	// 폰 위치와 폰이 속한 월드 가져오기
	FVector Center = HUDPawn->GetActorLocation();
	UWorld* World = HUDPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	// 모든 폰 감지하기
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
			// 폰들 중에 우리 플레이어인 경우
			if (EnemyPawn)
			{
				HpBar->SetMaxHp(EnemyPawn->GetMaxHP());
				EnemyPawn->SetupHUDWidget(this);
			}
		}
	}
}
